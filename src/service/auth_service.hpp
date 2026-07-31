#pragma once

#include "repositories/user_repository.hpp"
#include "dto/user_dto.hpp"
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/storages/postgres/cluster.hpp>

#include "dto/auth_dto.hpp"
#include "security/refresh_token_gen.hpp"
#include "repositories/refresh_token_repository.hpp"
#include "security/jwt_service.hpp"
namespace anxiety_backend {

class AuthService final : public userver::components::ComponentBase{
public:
    static constexpr std::string_view kName = "auth-service";

    AuthService (
        const userver::components::ComponentConfig& config,
        const userver::components::ComponentContext& context
    );

    AuthTokens Register (const UserDto& user) const;
    AuthTokens Login (const UserDto& user) const;
    // AuthTokens Refresh (const UserDto& user) const;

private:
    AuthTokens CreateTokens (std::string_view uuid) const;

    UserRepository user_repo_;
    RefreshTokenRepository refresh_token_repo_;
    const JwtService& jwt_;
    RefreshTokenGenerator refresh_token_gen_;
    

};

}