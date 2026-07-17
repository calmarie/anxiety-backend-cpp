#pragma once
#include <stdexcept>



class UserNotFoundError final : public std::runtime_error {
public:
    UserNotFoundError() : std::runtime_error("User not found") {}
};

