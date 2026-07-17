#include "user_repository.hpp"


namespace anxiety_backend {

    UserRepository::UserRepository(const userver::storages::postgres::ClusterPtr& cluster)
    : pg_cluster_(std::move(cluster)){} 

    void UserRepository::Create(const UserDto &user) const{
        std::string password_hash = "hardcoded_hash_123";
        const auto res = pg_cluster_->Execute(
            userver::storages::postgres::ClusterHostType::kMaster,
            "INSERT into users (email, name, password_hash) "
            "VALUES ($1,$2,$3)",
            user.email,
            user.name,
            password_hash
        );
    }


    std::optional<UserModel> UserRepository::GetInfo (std::string_view email) const{
        const auto res = pg_cluster_->Execute(
            userver::storages::postgres::ClusterHostType::kMaster,
            "SELECT id::text,email,name,created_at::text FROM users"
            " WHERE email = $1",
            email
        );

        const auto &row = res[0];

        UserModel u;

            u.id = row["id"].As<std::string>();
            u.email = row["email"].As<std::string>();
            u.name = row["name"].As<std::string>();
            u.created_at = row["created_at"].As<std::string>();

        return u;

    }

    bool UserRepository::Delete (std::string_view email) const{
        const auto res = pg_cluster_->Execute(
            userver::storages::postgres::ClusterHostType::kMaster,
            "DELETE FROM users"
            " WHERE email = $1",
            email
        );
        return res.RowsAffected() > 0;
    }

    bool UserRepository::UpdateName (std::string_view email, std::string_view name) const {

        const auto res = pg_cluster_->Execute(
            userver::storages::postgres::ClusterHostType::kMaster,
            "UPDATE users SET name = $1"
            " WHERE email = $2",
            name,
            email
        );

        return res.RowsAffected() > 0;
    }


}