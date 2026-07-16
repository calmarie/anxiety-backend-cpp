#pragma once

#include <userver/storages/postgres/cluster.hpp>

#include "models/user.hpp"

namespace anxiety_backend {
    class UserRepository{
    public:

        explicit UserRepository(const userver::storages::postgres::ClusterPtr& cluster);

        void Create (const UserInfo &user) const;
        std::optional<UserInfo> GetInfo (std::string_view email) const;
        bool Delete (std::string_view email) const;
        bool UpdateName (std::string_view email, std::string_view name) const;


    private:
        
        userver::storages::postgres::ClusterPtr pg_cluster_;

    };
}