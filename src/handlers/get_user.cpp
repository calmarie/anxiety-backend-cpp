#include "get_user.hpp"
#include <userver/storages/postgres/component.hpp>
#include <stdexcept>

namespace anxiety_backend {

GetUserHandler::GetUserHandler   
(   
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& component_context
)
    : HttpHandlerBase(config, component_context),
      user_service_(component_context.FindComponent<UserService>())
{}

std::string GetUserHandler::HandleRequestThrow(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext&  
)  const {

    

    const auto email = request.GetPathArg("email");
    auto& response_сode = request.GetHttpResponse();

    userver::formats::json::ValueBuilder response;
    try
    {
    auto user_info = user_service_.GetUser(email);

    response["id"] = user_info.id;
    response["email"] = user_info.email;
    response["name"] = user_info.name;
    response["created_at"] = user_info.created_at;
    }
    catch (std::runtime_error& err)
    {
        response["error"] = err.what();
        response_сode.SetStatus(userver::server::http::HttpStatus::kNotFound);
    }
    catch (std::invalid_argument& err)
    {
        response["error"] = err.what();
        response_сode.SetStatus(userver::server::http::HttpStatus::kBadRequest);
    }

    return userver::formats::json::ToString(response.ExtractValue());

}

}
