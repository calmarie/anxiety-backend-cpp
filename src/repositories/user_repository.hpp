#pragma once

#include "security/password_hasher.hpp"


#include <userver/storages/postgres/cluster.hpp>

#include "models/user_model.hpp"
#include "dto/user_dto.hpp"

namespace anxiety_backend {
    class UserRepository final{
    public:

        explicit UserRepository(const userver::storages::postgres::ClusterPtr& cluster);

        void Create (const UserDto &user) const;
        std::optional<UserModel> GetInfo (std::string_view email) const;
        bool Delete (std::string_view email) const;
        bool UpdateName (std::string_view email, std::string_view name) const;

        std::optional<std::string> GetHash (std::string_view hash) const;


    private:
        
        PasswordHasher hasher_;
        userver::storages::postgres::ClusterPtr pg_cluster_;

    };
}