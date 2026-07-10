#include "get_user.hpp"
#include <greeting.hpp>
#include <userver/storages/postgres/component.hpp>

namespace anxiety_backend {

GetUsers::GetUsers     
(   
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& component_context
)
    : HttpHandlerBase(config, component_context),
      pg_cluster_(component_context.FindComponent<userver::components::Postgres>("postgres-db-1").GetCluster())
{}

std::string GetUsers::HandleRequestThrow(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext&  
)  const {
    
    const auto email = request.GetPathArg("email");
    auto& responseCode = request.GetHttpResponse();
    if (email.empty()){
        responseCode.SetStatus(userver::server::http::HttpStatus::kBadRequest);
        return "id is required";
    }
    const auto res = pg_cluster_->Execute(
    userver::storages::postgres::ClusterHostType::kMaster,
    "SELECT id::text,email,name,created_at::text FROM users"
    " WHERE email = $1",
    email
    );
    if (res.IsEmpty()){
        LOG_INFO() << "USER NOT FOUND";

        responseCode.SetStatus(userver::server::http::HttpStatus::NotFound);
        return "Incorrect id";
        }
    
    // std::vector<UserInfo> users;
    userver::formats::json::ValueBuilder response;
    response.Resize(0);
    userver::formats::json::ValueBuilder user;
    const auto &row = res[0];
    
    user["id"] = row["id"].As<std::string>();
    user["email"] = row["email"].As<std::string>();
    user["name"] = row["name"].As<std::string>();
    user["created_at"] = row["created_at"].As<std::string>();

    response.PushBack(user.ExtractValue());
        
    return userver::formats::json::ToString(response.ExtractValue());

}

}
