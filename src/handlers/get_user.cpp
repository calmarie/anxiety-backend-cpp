#include "get_user.hpp"
#include "repositories/user_repository.hpp"

#include <userver/storages/postgres/component.hpp>

namespace anxiety_backend {

GetUser::GetUser   
(   
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& component_context
)
    : HttpHandlerBase(config, component_context),
      pg_cluster_(component_context.FindComponent<userver::components::Postgres>("postgres-db-1").GetCluster())
{}

std::string GetUser::HandleRequestThrow(
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
    auto userInfo = repo.GetInfo(email);
    if (!userInfo){
        LOG_INFO() << "USER NOT FOUND";

        responseCode.SetStatus(userver::server::http::HttpStatus::NotFound);
        return "Incorrect email";
        }
    
    // std::vector<UserInfo> users;
    userver::formats::json::ValueBuilder response;
    response.Resize(0);
    userver::formats::json::ValueBuilder user;

    user["id"] = userInfo->id;
    user["email"] = userInfo->email;
    user["name"] = userInfo->name;
    user["created_at"] = userInfo->created_at;

    response.PushBack(user.ExtractValue());
        
    return userver::formats::json::ToString(response.ExtractValue());

}

}
