#pragma once
#include "models/user_model.hpp"
#include "dto/auth_dto.hpp"
#include <userver/server/handlers/http_handler_base.hpp>

namespace anxiety_backend {

    userver::formats::json::Value SerilizeUser (const UserModel& user);
    userver::formats::json::Value SerilizeError (const char* error);
    userver::formats::json::Value SerilizeTokens (const AuthTokens& tokens);

}