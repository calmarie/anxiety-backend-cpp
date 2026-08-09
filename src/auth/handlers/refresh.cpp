#include "refresh.hpp"

#include "serializers/user_serializer.hpp"

namespace anxiety_backend {

RefreshTokensHandler::RefreshTokensHandler     
(   
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& component_context
)
    : HttpHandlerBase(config, component_context),
      auth_service_(component_context.FindComponent<AuthService>())
{}

std::string RefreshTokensHandler::HandleRequestThrow(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext&  
)  const {
    
    auto json = userver::formats::json::FromString(request.RequestBody());
    auto refresh_token = json["refresh_token"].As<std::string>();

    auto& response_code = request.GetHttpResponse();
    
    userver::formats::json::Value response;
    try {
        auto tokens = auth_service_.Refresh(refresh_token);
        response = SerilizeTokens(tokens);
        response_code.SetStatus(userver::server::http::HttpStatus::kCreated);
    }
    catch (std::runtime_error& err)
    {
        response = SerilizeError(err.what());
        response_code.SetStatus(userver::server::http::HttpStatus::kConflict);
    }

    return userver::formats::json::ToString(response);

}

}