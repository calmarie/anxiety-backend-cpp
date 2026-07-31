#include "refresh_token_repository.hpp"
#include <userver/storages/postgres/io/time_of_day.hpp>
#include <chrono>

namespace anxiety_backend {

    RefreshTokenRepository::RefreshTokenRepository(
        const userver::storages::postgres::ClusterPtr& cluster
    )
    : pg_cluster_(std::move(cluster)){} 

    bool RefreshTokenRepository::SaveRefreshToken(std::string_view token_hash, std::string_view user_id) const{
        auto created_at = userver::storages::postgres::TimePointTz{std::chrono::system_clock::now()};
        auto expires_at = userver::storages::postgres::TimePointTz{std::chrono::system_clock::now() + std::chrono::days{30}};
        const auto res = pg_cluster_->Execute(
            userver::storages::postgres::ClusterHostType::kMaster,
            "INSERT into refresh_tokens "
            "(user_id, token_hash, created_at, expires_at) "
            "VALUES ($1::uuid,$2,$3,$4)",
            user_id,
            token_hash,
            created_at,
            expires_at
        );

        return res.RowsAffected() > 0;
    }

    // std::string GetTokenHash () const {

    //     const auto res = pg_cluster_->Execute(
    //         userver::storages::postgres::ClusterHostType::kMaster,
    //         "DELETE from refresh_tokens "
    //         "WHERE token_hash = $1 "
    //         "AND expires_at > NOW() "
    //         "RETURNING user_id;",
    //     );

    //     return res.AsSingleRow<res>;

    // }

}