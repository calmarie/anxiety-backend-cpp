#pragma once

#include <string>

#include <jwt-cpp/jwt.h>

#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/components/component.hpp>

#define TOKENLEN 32

namespace anxiety_backend {

class RefreshTokenGenerator final{
public:
    std::string GenerateRefreshToken () const;
    std::string HashRefreshToken (std::string_view token) const;
};

}