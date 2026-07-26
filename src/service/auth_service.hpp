#pragma once

#include "repositories/user_repository.hpp"
#include "dto/user_dto.hpp"
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/storages/postgres/cluster.hpp>

#include "security/jwt_service.hpp"
namespace anxiety_backend {

class AuthService final : public userver::components::ComponentBase{
public:
    static constexpr std::string_view kName = "auth-service";

    AuthService (
        const userver::components::ComponentConfig& config,
        const userver::components::ComponentContext& context
    );

    std::string Register (const UserDto& user) const;
    std::string Login (const UserDto& user) const;

private:
    UserRepository repo_;
    const JwtService& jwt_;

};

}