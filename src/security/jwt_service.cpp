#include "jwt_service.hpp"

#include <chrono>
#include <userver/yaml_config/merge_schemas.hpp>

namespace anxiety_backend {

JwtService::JwtService(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context
)
    : ComponentBase(config, context), secret_(config["secret"].As<std::string>())
{}

userver::yaml_config::Schema JwtService::GetStaticConfigSchema() {
    return userver::yaml_config::MergeSchemas<
        userver::components::ComponentBase
    >(R"(
    type: object
    description: JWT service configuration
    additionalProperties: false
    properties:
        secret:
            type: string
            description: Secret key to sign JWT tokens
    )");
}

std::string JwtService::CreateToken (std::string_view uuid) const{
    auto expiration = std::chrono::minutes{60};

    auto token = jwt::create()
    .set_type("JWS")
    .set_subject(std::string(uuid))
    .set_issuer("anxiety_backend")
    .set_issued_now()
    .set_expires_in(expiration)
    .sign(jwt::algorithm::hs256{secret_});

    return token;

}

bool JwtService::VerifyToken (std::string_view token) const{

    try {
        std::error_code ec;
        auto veryfier = jwt::verify()
        .with_issuer("anxiety_backend")
        .allow_algorithm(jwt::algorithm::hs256{secret_});

        veryfier.verify(jwt::decode(std::string(token)));

        return true;
    }
    catch (jwt::error::token_verification_error& e){
        return false;
    }


}

}
