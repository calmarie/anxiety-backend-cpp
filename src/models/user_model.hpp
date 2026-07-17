#pragma once

#include <string>

struct UserModel{
    std::string id;
    std::string email;
    std::string name;
    std::string password_hash;
    std::string created_at;
};

