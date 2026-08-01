#pragma once
#include <stdexcept>



class WrongRefreshTokenError final : public std::runtime_error {
public:
    WrongRefreshTokenError () : std::runtime_error("Wrong refresh token") {}
};