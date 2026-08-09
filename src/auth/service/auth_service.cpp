#include "auth_service.hpp"

#include <userver/components/component.hpp>

#include "exceptions/email_already_exists_error.hpp"
#include "exceptions/email_is_required_error.hpp"
#include "exceptions/incorrect_password_or_email_error.hpp"
#include "exceptions/redis_refresh_token_error.hpp"
#include "exceptions/refresh_token_is_required_error.hpp"

#include "security/password_hasher.hpp"
#include "security/refresh_token_gen.hpp"

namespace anxiety_backend {
    AuthService::AuthService (
        const userver::components::ComponentConfig& config,
        const userver::components::ComponentContext& context
    ) : ComponentBase(config, context),
        user_repo_ (
            context
        ),
        refresh_token_repo_ (
            context
        ),
        jwt_(context.FindComponent<JwtService>())
    {}

    AuthTokens AuthService::Register (const UserDto& user) const{
        try {
            user_repo_.Create(user);
        }
        catch (const userver::storages::postgres::UniqueViolation&) {
             throw EmailAlreadyExistsError();
        }

        auto user_info = *user_repo_.GetInfo(user.email);  

        return CreateTokens(user_info.uuid);

    }

    AuthTokens AuthService::Login (const UserDto& user) const{

    if (user.email.empty() || user.password.empty()) throw EmailIsRequiredError();
    
    auto password_hash = user_repo_.GetHash(user.email);
    
    if (password_hash->empty())
        throw IncorrectPasswordOrEmailError();
    
    PasswordHasher hasher;
    auto flag = hasher.Verify(*password_hash, user.password);
    
    if (flag){
        throw IncorrectPasswordOrEmailError();
    }
    auto user_info = *user_repo_.GetInfo(user.email);  
    
    return  CreateTokens(user_info.uuid);

    }

    AuthTokens AuthService::CreateTokens (std::string_view uuid) const{
        AuthTokens tokens;
        tokens.access_token = jwt_.CreateAccessToken(uuid);
        tokens.refresh_token = refresh_token_gen_.GenerateRefreshToken();
        refresh_token_repo_.SaveRefreshToken(
            refresh_token_gen_.HashRefreshToken(tokens.refresh_token),
            uuid
        );

        
        //     throw RefreshTokenCreationError();

        return tokens;

    }

    AuthTokens AuthService::Refresh (std::string_view refresh_token) const{

        auto refresh_token_hash = refresh_token_gen_.
        HashRefreshToken(refresh_token);

        if (refresh_token.empty()){
            throw RefreshTokenIsRequiredError();
        }

        auto user_id = refresh_token_repo_.
        VerifyToken(refresh_token_hash);

        if (!user_id.has_value()){
            throw RedisRefreshTokenError();
        }

        refresh_token_repo_.RecallToken(refresh_token_hash);  

        return CreateTokens(*user_id);

    }

    void AuthService::Logout (std::string_view refresh_token) const{

        if (refresh_token.empty()){
            throw RefreshTokenIsRequiredError();
        }

        auto refresh_token_hash = refresh_token_gen_.
        HashRefreshToken(refresh_token);

        auto flag = refresh_token_repo_.RecallToken(refresh_token_hash);

        if (!flag){
            throw RedisRefreshTokenError();
        }
        
    }
}