#pragma once

#include "security/refresh_token_gen.hpp"
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/redis/component.hpp>
#include <userver/storages/redis/client.hpp>
#include <userver/components/component.hpp>
namespace anxiety_backend {
    class RefreshTokenRepository final{
    public:

        explicit RefreshTokenRepository(const userver::components::ComponentContext& context);
        
        void SaveRefreshToken(std::string_view token_hash, std::string_view user_id) const;
        std::optional<std::string> VerifyToken(std::string_view token_hash) const;
        bool RecallToken (std::string_view token_hash) const;

    private:    
        
        RefreshTokenGenerator token_generator_;
        userver::storages::redis::ClientPtr redis_client_;
        userver::storages::redis::CommandControl redis_cc_;
    };
}