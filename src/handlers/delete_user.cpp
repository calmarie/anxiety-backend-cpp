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
      pg_cluster_(component_context.FindComponent<userver::components::Postgres>("postgres-db-1").GetCluster())
{}

std::string DeleteUser::HandleRequestThrow(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext&  
)  const {
    
    const auto email = request.GetPathArg("email");
    auto& responseCode = request.GetHttpResponse();
    if (email.empty()){
        responseCode.SetStatus(userver::server::http::HttpStatus::kBadRequest);
        return "email is required";
    }
    UserRepository repo(pg_cluster_);
    auto flag = repo.Delete(email);

    if (flag == 0){

        responseCode.SetStatus(userver::server::http::HttpStatus::NotFound);
        return "Incorrect email";
        }
    
    responseCode.SetStatus(userver::server::http::HttpStatus::kNoContent);
    return "The user is successfully deleted";

}

}
