#include "login.hpp"
#include "serializers/user_serializer.hpp"

#include <userver/storages/postgres/component.hpp>
#include <stdexcept>

namespace anxiety_backend {

LogInUserHandler::LogInUserHandler   
(   
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& component_context
)
    : HttpHandlerBase(config, component_context),
      auth_service_(component_context.FindComponent<AuthService>())
{}

std::string LogInUserHandler::HandleRequestThrow(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext&  
)  const {

    
    std::string body = request.RequestBody();
    UserDto dto = Deserialize(body);

    userver::formats::json::Value response;
    auto& response_сode = request.GetHttpResponse();

    try
    {
        auto access_token = auth_service_.Login(dto);
        response = SerilizeJwt(access_token);
    }
    catch (std::runtime_error& err)
    {
        response = SerilizeError(err.what());
        response_сode.SetStatus(userver::server::http::HttpStatus::kUnauthorized);
    }
    catch (std::invalid_argument& err)
    {
        response = SerilizeError(err.what());
        response_сode.SetStatus(userver::server::http::HttpStatus::kBadRequest);
    }

    return userver::formats::json::ToString(response);

}

UserDto LogInUserHandler::Deserialize (std::string body) const{
    auto json = userver::formats::json::FromString(body);
    UserDto dto;

    dto.email = json["email"].As<std::string>();
    dto.password = json["password"].As<std::string>();

    return dto;

}

}