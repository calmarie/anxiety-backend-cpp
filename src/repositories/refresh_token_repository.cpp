#include "refresh_token_repository.hpp"
#include <userver/storages/postgres/io/time_of_day.hpp>
#include <chrono>

#include "exceptions/redis_refresh_token_error.hpp"

namespace anxiety_backend {

    RefreshTokenRepository::RefreshTokenRepository(
        const userver::components::ComponentContext& context
    )
    : redis_client_(context.
        FindComponent<userver::components::Redis>("redis-db-1")
        .GetClient("refresh-tokens")
    ) ,
    redis_cc_ (
        std::chrono::milliseconds{500},
        std::chrono::milliseconds{1000},
        2
    ) {}


    void RefreshTokenRepository::SaveRefreshToken(std::string_view token_hash, std::string_view user_id) const{

        try{

        redis_client_->Setex(
            "refresh_token:"+std::string(token_hash),
            std::chrono::seconds{30*24*60*60},
            std::string(user_id),
            redis_cc_
        ).Get();

        }
        catch (const userver::storages::redis::RequestFailedException& e){
            throw (LOG_ERROR() << "Redis error: " << e);
        }

    }

    std::optional<std::string> RefreshTokenRepository::VerifyToken (
        std::string_view token_hash
    ) const {

        auto res = redis_client_->Get(
            "refresh_token:"+std::string(token_hash),
            redis_cc_
        ).Get();

        if (!res){
            throw (RedisRefreshTokenError());
        }

        return (res);
        
    }

}