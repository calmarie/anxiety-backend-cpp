#pragma once

#include "security/refresh_token_gen.hpp"
#include <userver/storages/postgres/cluster.hpp>

namespace anxiety_backend {
    class RefreshTokenRepository final{
    public:

        explicit RefreshTokenRepository(const userver::storages::postgres::ClusterPtr& cluster);

        bool SaveRefreshToken(std::string_view token_hash, std::string_view user_id) const;
        // std::string GetTokenHash () const;

    private:
        
        RefreshTokenGenerator token_generator_;
        userver::storages::postgres::ClusterPtr pg_cluster_;

    };
}