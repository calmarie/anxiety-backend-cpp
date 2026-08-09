#pragma once
#include <stdexcept>



class RefreshTokenIsRequiredError final : public std::invalid_argument {
public:
    RefreshTokenIsRequiredError() : std::invalid_argument("Refresh token is required") {}
};

