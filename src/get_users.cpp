#include "get_users.hpp"
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
    const userver::server::http::HttpRequest&,
    userver::server::request::RequestContext& 
)  const {    
    const auto res = pg_cluster_->Execute(
    userver::storages::postgres::ClusterHostType::kMaster,
    "SELECT id::text,email,name,created_at::text FROM users LIMIT 10"
    );
    
    std::vector<UserInfo> users;
    for (const auto &row : res){

        UserInfo u;

        u.id = row["id"].As<std::string>();
        u.email = row["email"].As<std::string>();
        u.name = row["name"].As<std::string>();
        u.created_at = row["created_at"].As<std::string>();

        users.push_back(u);

    }
    
    userver::formats::json::ValueBuilder response;
    response.Resize(0);
    
    for (const auto &u : users){
        
        userver::formats::json::ValueBuilder user;

        user["id"] = u.id;
        user["email"] = u.email;
        user["name"] = u.name;
        user["created_at"] = u.name;

        response.PushBack(user.ExtractValue());
        
    }

    return userver::formats::json::ToString(response.ExtractValue());

}

}
