#pragma once

#include <string>

#include <jwt-cpp/jwt.h>

#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/components/component.hpp>

namespace anxiety_backend {

class JwtService final : public userver::components::ComponentBase{
public:
    static constexpr std::string_view kName = "jwt-service";
    JwtService(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context
    );
    std::string CreateToken (std::string_view uuid) const;
    bool VerifyToken (std::string_view token) const;

    static userver::yaml_config::Schema GetStaticConfigSchema();
private:
    std::string secret_;
};

}