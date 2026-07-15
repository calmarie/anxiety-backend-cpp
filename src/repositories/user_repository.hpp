#pragma once

#include <userver/storages/postgres/cluster.hpp>

#include "models/user.hpp"

namespace anxiety_backend {
    class UserRepository{
    public:

        explicit UserRepository(userver::storages::postgres::ClusterPtr cluster);

        void Create (const UserInfo &user) const;
        std::optional<UserInfo> GetInfo (const std::string &email) const;
        bool Delete (const std::string &email) const;
        bool UpdateName (const std::string &email, const std::string &name) const;


    private:
        
        userver::storages::postgres::ClusterPtr pg_cluster_;

    };
}