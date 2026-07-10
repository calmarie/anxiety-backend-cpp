#include "set_user.hpp"
#include <greeting.hpp>
#include <userver/storages/postgres/component.hpp>

namespace anxiety_backend {

SetUser::SetUser     
(   
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& component_context
)
    : HttpHandlerBase(config, component_context),
      pg_cluster_(component_context.FindComponent<userver::components::Postgres>("postgres-db-1").GetCluster())
{}

std::string SetUser::HandleRequestThrow(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext&  
)  const {
    
    std::string body = request.RequestBody();
    UserInfo user = createUser(body);

   
    try {
        insertUser(user);
    }
    catch (const userver::storages::postgres::UniqueViolation&) {

        request.GetHttpResponse().SetStatus(
            userver::server::http::HttpStatus::kConflict
        );

        return R"({"error":"email already exists"})";
    }
    
    return getUserInfo(user.email);


}



UserInfo SetUser::createUser (std::string body) {
    auto json = userver::formats::json::FromString(body);
    UserInfo user;

    user.email = json["email"].As<std::string>();
    user.name = json["name"].As<std::string>();
    user.password_hash = "hardcoded_hash_123";

    return user;

}

void SetUser::insertUser (UserInfo user) const{

    const auto res = pg_cluster_->Execute(
    userver::storages::postgres::ClusterHostType::kMaster,
    "INSERT into users (email, name, password_hash) "
    "VALUES ($1,$2,$3)",
    user.email,
    user.name,
    user.password_hash
    );

}

std::string SetUser::getUserInfo (std::string email) const{

    const auto res = pg_cluster_->Execute(
    userver::storages::postgres::ClusterHostType::kMaster,
    "SELECT id::text FROM users"
    " WHERE email = $1",
    email
    );

    return res.AsSingleRow<std::string>();

}
}
