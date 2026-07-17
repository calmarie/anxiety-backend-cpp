#include "user_service.hpp"

#include <stdexcept>

#include <userver/components/component.hpp>
#include <userver/storages/postgres/component.hpp>

namespace anxiety_backend{


    UserService::UserService (
        const userver::components::ComponentConfig& config,
        const userver::components::ComponentContext& context
    ) : ComponentBase(config, context),
        repo_ (
            context.FindComponent<userver::components::Postgres>(
                "postgres-db-1"
            ).GetCluster()
        )
    {}


    UserInfo UserService::GetUser (std::string_view email) const
    {

        if (email.empty()) throw std::invalid_argument("Email is required");
        
        auto userInfo = repo_.GetInfo(email);

        if (!userInfo) throw std::runtime_error("User not found");

        return *userInfo;
    
    }

    void UserService::DeleteUser (std::string_view email) const {

        if (email.empty()) throw std::invalid_argument("Email is required");
        
        auto flag = repo_.Delete(email);

        if (!flag) throw std::runtime_error("User not found");

    }

    UserInfo UserService::UpdateUserName (std::string_view email, std::string_view name) const {

        if (email.empty()) throw std::invalid_argument("Email is required");

        auto flag = repo_.UpdateName(email, name);

        if (!flag) throw std::runtime_error("User not found");

        return *repo_.GetInfo(email);  

    }

    UserInfo UserService::CreateUser (const UserInfo& user) const {

        try
        {
            repo_.Create(user);
        }

        catch (const userver::storages::postgres::UniqueViolation&) {
            throw std::runtime_error("Email already exists");
        }

        return *repo_.GetInfo(user.email);  
    }






}