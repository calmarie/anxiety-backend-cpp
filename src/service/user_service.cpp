#include "user_service.hpp"

#include "exceptions/user_not_found_error.hpp"
#include "exceptions/email_already_exists_error.hpp"
#include "exceptions/email_is_required_error.hpp"

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


    UserModel UserService::GetUser (std::string_view email) const
    {

        if (email.empty()) throw EmailIsRequiredError();
        
        auto userInfo = repo_.GetInfo(email);

        if (!userInfo) throw UserNotFoundError();

        return *userInfo;
    
    }

    void UserService::DeleteUser (std::string_view email) const {

        if (email.empty()) throw EmailIsRequiredError();
        
        auto flag = repo_.Delete(email);

        if (!flag) throw UserNotFoundError();

    }

    UserModel UserService::UpdateUserName (std::string_view email, std::string_view name) const {

        if (email.empty()) throw EmailIsRequiredError();

        auto flag = repo_.UpdateName(email, name);

        if (!flag) throw UserNotFoundError();

        return *repo_.GetInfo(email);  

    }

    UserModel UserService::CreateUser (const UserDto& user) const {

        try
        {
            repo_.Create(user);
        }

        catch (const userver::storages::postgres::UniqueViolation&) {
            throw EmailAlreadyExistsError();
        }

        return *repo_.GetInfo(user.email);  
    }






}