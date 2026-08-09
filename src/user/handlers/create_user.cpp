#include "create_user.hpp"

#include "serializers/user_serializer.hpp"

#include <userver/storages/postgres/component.hpp>

namespace anxiety_backend {

CreateUserHandler::CreateUserHandler     
(   
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& component_context
)
    : HttpHandlerBase(config, component_context),
      user_service_(component_context.FindComponent<UserService>())
{}

std::string CreateUserHandler::HandleRequestThrow(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext&  
)  const {
    
    std::string body = request.RequestBody();
    UserDto dto = Deserialize(body);

    auto& response_code = request.GetHttpResponse();
    
    userver::formats::json::Value response;
    try {
        auto user_info = user_service_.CreateUser(dto);

        response = SerilizeUser(user_info);

        response_code.SetStatus(userver::server::http::HttpStatus::kCreated);
    }
    catch (std::runtime_error& err)
    {
        response = SerilizeError(err.what());
        response_code.SetStatus(userver::server::http::HttpStatus::kConflict);
    }

    return userver::formats::json::ToString(response);

}



UserDto CreateUserHandler::Deserialize (std::string body) const{
    auto json = userver::formats::json::FromString(body);
    UserDto dto;

    dto.email = json["email"].As<std::string>();
    dto.name = json["name"].As<std::string>();
    dto.password = json["password"].As<std::string>();

    return dto;

}

}
