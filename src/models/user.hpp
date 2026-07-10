#pragma once

#include <string>
// #include <userver/components/component.hpp>
// #include <userver/server/handlers/http_handler_base.hpp>

// #include <userver/storages/postgres/cluster.hpp>

struct UserInfo{
    std::string id;
    std::string email;
    std::string name;
    std::string password_hash;
    std::string created_at;
};

