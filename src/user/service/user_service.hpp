#pragma once

#include "user/repositories/user_repository.hpp"
#include "models/user_model.hpp"
#include "dto/user_dto.hpp"
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/storages/postgres/cluster.hpp>



namespace anxiety_backend {

class UserService final : public userver::components::ComponentBase{
public:
    static constexpr std::string_view kName = "user-service";

    UserService (
        const userver::components::ComponentConfig& config,
        const userver::components::ComponentContext& context
    );

    UserModel GetUser (std::string_view email) const;
    void DeleteUser (std::string_view email) const;
    UserModel UpdateUserName (std::string_view email, std::string_view name) const;
    UserModel CreateUser (const UserDto& user) const;

private:
    UserRepository repo_;
};

}