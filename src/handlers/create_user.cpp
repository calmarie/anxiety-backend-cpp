#include "create_user.hpp"

#include <userver/storages/postgres/component.hpp>

namespace anxiety_backend {

CreateUser::CreateUser     
(   
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& component_context
)
    : HttpHandlerBase(config, component_context),
      user_service_(component_context.FindComponent<UserService>())
{}

std::string CreateUser::HandleRequestThrow(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext&  
)  const {
    
    std::string body = request.RequestBody();
    UserInfo user = createUser(body);

    auto& response_code = request.GetHttpResponse();
    
    userver::formats::json::ValueBuilder response;
    try {
        auto user_info = user_service_.CreateUser(user);

        response["id"] = user_info.id;
        response["email"] = user_info.email;
        response["name"] = user_info.name;
        response["created_at"] = user_info.created_at;

        response_code.SetStatus(userver::server::http::HttpStatus::kCreated);
    }
    catch (std::runtime_error& err)
    {
        response["error"] = err.what();
        response_code.SetStatus(userver::server::http::HttpStatus::kConflict);
    }

    return userver::formats::json::ToString(response.ExtractValue());

}



UserInfo CreateUser::createUser (std::string body) {
    auto json = userver::formats::json::FromString(body);
    UserInfo user;

    user.email = json["email"].As<std::string>();
    user.name = json["name"].As<std::string>();
    user.password_hash = "hardcoded_hash_123";

    return user;

}

}
