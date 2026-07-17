#include "update_user_name.hpp"

#include "serializers/user_serializer.hpp"

#include <userver/storages/postgres/component.hpp>

namespace anxiety_backend {

UpdateUserNameHandler::UpdateUserNameHandler   
(   
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& component_context
)
    : HttpHandlerBase(config, component_context),
      user_service_(component_context.FindComponent<UserService>())
{}

std::string UpdateUserNameHandler::HandleRequestThrow(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext&  
)  const {
    
    auto json = userver::formats::json::FromString(request.RequestBody());
    auto email = json["email"].As<std::string>();
    auto name = json["name"].As<std::string>();

    auto& response_сode = request.GetHttpResponse();


    response_сode.SetStatus(userver::server::http::HttpStatus::kBadRequest);
    
    userver::formats::json::Value response;

    try
    {
    auto user_info = user_service_.UpdateUserName(email, name);
    
    response = SerilizeUser(user_info);

    }
    catch (std::runtime_error& err)
    {
        response = SerilizeError(err.what());
        response_сode.SetStatus(userver::server::http::HttpStatus::kNotFound);
    }
    catch (std::invalid_argument& err)
    {
        response = SerilizeError(err.what());
        response_сode.SetStatus(userver::server::http::HttpStatus::kBadRequest);
    }

    return userver::formats::json::ToString(response);

}
}
