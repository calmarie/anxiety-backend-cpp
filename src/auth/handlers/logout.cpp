#include "logout.hpp"
#include "serializers/user_serializer.hpp"

#include <userver/storages/postgres/component.hpp>
#include <stdexcept>

namespace anxiety_backend {

LogoutHandler::LogoutHandler   
(   
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& component_context
)
    : HttpHandlerBase(config, component_context),
      auth_service_(component_context.FindComponent<AuthService>())
{}

std::string LogoutHandler::HandleRequestThrow(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext&  
)  const {

    
    auto json = userver::formats::json::FromString(request.RequestBody());
    auto refresh_token = json["refresh_token"].As<std::string>();

    auto& response_code = request.GetHttpResponse();
    userver::formats::json::Value response;
    try
    {   
        auth_service_.Logout(refresh_token);
        response_code.SetStatus(userver::server::http::HttpStatus::kNoContent);
    }
    catch (std::runtime_error& err)
    {
        response = SerilizeError(err.what());
        response_code.SetStatus(userver::server::http::HttpStatus::kUnauthorized);
    }
    catch (std::invalid_argument& err)
    {
        response = SerilizeError(err.what());
        response_code.SetStatus(userver::server::http::HttpStatus::kBadRequest);
    }

    return userver::formats::json::ToString(response);

}


}