#include "delete_user.hpp"
#include "repositories/user_repository.hpp"

#include <userver/storages/postgres/component.hpp>

namespace anxiety_backend {

DeleteUser::DeleteUser     
(   
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& component_context
)
    : HttpHandlerBase(config, component_context),
      user_service_(component_context.FindComponent<userver::components::Postgres>("postgres-db-1").GetCluster())
{}

std::string DeleteUser::HandleRequestThrow(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext&  
)  const {
    
    const auto email = request.GetPathArg("email");
    auto& response_code = request.GetHttpResponse();

    userver::formats::json::ValueBuilder response;


    try {
        user_service_.DeleteUser(email);
        response_code.SetStatus(userver::server::http::HttpStatus::kNoContent);
        response["success"] = "success";
    }
    catch (std::invalid_argument& err){
        response["error"] = err.what();
        response_code.SetStatus(userver::server::http::HttpStatus::kBadRequest);
    }
    catch (std::runtime_error& err){
         response["error"] = err.what();
        response_code.SetStatus(userver::server::http::HttpStatus::kNotFound);
    }

    return userver::formats::json::ToString(response.ExtractValue());

}

}
