#pragma once
#include <stdexcept>



class EmailAlreadyExistsError final : public std::runtime_error {
public:
    EmailAlreadyExistsError() : std::runtime_error("Email already exists") {}
};

