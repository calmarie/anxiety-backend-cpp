#pragma once
#include <stdexcept>

#include <userver/storages/redis/client.hpp>

class RedisRefreshTokenError final : public std::runtime_error {
public:
    RedisRefreshTokenError () : std::runtime_error("Wrong refresh token") {}
};