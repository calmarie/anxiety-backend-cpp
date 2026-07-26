#pragma once
#include <stdexcept>



class IncorrectPasswordOrEmailError final : public std::runtime_error {
public:
    IncorrectPasswordOrEmailError() : std::runtime_error("Incorrect password or email") {}
};

