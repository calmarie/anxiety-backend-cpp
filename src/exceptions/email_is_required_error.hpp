#pragma once
#include <stdexcept>



class EmailIsRequiredError final : public std::invalid_argument {
public:
    EmailIsRequiredError() : std::invalid_argument("Email is required") {}
};

