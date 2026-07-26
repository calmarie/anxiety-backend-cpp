#include "auth_service.hpp"

#include <userver/components/component.hpp>
#include <userver/storages/postgres/component.hpp>


#include "exceptions/email_already_exists_error.hpp"
#include "exceptions/email_is_required_error.hpp"
#include "exceptions/incorrect_password_or_email_error.hpp"

#include "security/password_hasher.hpp"


namespace anxiety_backend {
    AuthService::AuthService (
        const userver::components::ComponentConfig& config,
        const userver::components::ComponentContext& context
    ) : ComponentBase(config, context),
        repo_ (
            context.FindComponent<userver::components::Postgres>(
                "postgres-db-1"
            ).GetCluster()
        ),
        jwt_(context.FindComponent<JwtService>())
    {}

    std::string AuthService::Register (const UserDto& user) const{
        try {
            repo_.Create(user);
        }
        catch (const userver::storages::postgres::UniqueViolation&) {
             throw EmailAlreadyExistsError();
        }

        auto user_info = *repo_.GetInfo(user.email);  

        std::string token = jwt_.CreateToken(user_info.uuid);
        return token;

        

    }

    std::string AuthService::Login (const UserDto& user) const{

    if (user.email.empty() || user.password.empty()) throw EmailIsRequiredError();
    
    auto password_hash = repo_.GetHash(user.email);
    
    if (password_hash->empty())
        throw IncorrectPasswordOrEmailError();
    
    PasswordHasher hasher;
    auto flag = hasher.Verify(*password_hash, user.password);
    
    if (flag){
        throw IncorrectPasswordOrEmailError();
    }
    auto user_info = *repo_.GetInfo(user.email);  
    
    return  jwt_.CreateToken(user_info.uuid);

    }
}