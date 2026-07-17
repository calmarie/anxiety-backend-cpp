#include "delete_user.hpp"

#include "serializers/user_serializer.hpp"

#include <userver/storages/postgres/component.hpp>

namespace anxiety_backend {

DeleteUserHandler::DeleteUserHandler     
(   
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& component_context
)
    : HttpHandlerBase(config, component_context),
      user_service_(component_context.FindComponent<UserService>())
{}

std::string DeleteUserHandler::HandleRequestThrow(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext&  
)  const {
    
    const auto email = request.GetPathArg("email");
    auto& response_code = request.GetHttpResponse();

    userver::formats::json::Value response;


    try {
        user_service_.DeleteUser(email);
        response_code.SetStatus(userver::server::http::HttpStatus::kNoContent);
    }
    catch (std::invalid_argument& err){
        response = SerilizeError(err.what());
        response_code.SetStatus(userver::server::http::HttpStatus::kBadRequest);
    }
    catch (std::runtime_error& err){
        response = SerilizeError(err.what());
        response_code.SetStatus(userver::server::http::HttpStatus::kNotFound);
    }

    return userver::formats::json::ToString(response);

}

}
