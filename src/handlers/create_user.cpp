#include "create_user.hpp"
#include "repositories/user_repository.hpp"

#include <userver/storages/postgres/component.hpp>

namespace anxiety_backend {

CreateUser::CreateUser     
(   
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& component_context
)
    : HttpHandlerBase(config, component_context),
      pg_cluster_(component_context.FindComponent<userver::components::Postgres>("postgres-db-1").GetCluster())
{}

std::string CreateUser::HandleRequestThrow(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext&  
)  const {
    
    std::string body = request.RequestBody();
    UserInfo user = createUser(body);

    UserRepository repo(pg_cluster_);
    try {
        repo.Create(user);
    }
    catch (const userver::storages::postgres::UniqueViolation&) {

        request.GetHttpResponse().SetStatus(
            userver::server::http::HttpStatus::kConflict
        );

        return R"({"error":"email already exists"})";
    }
    auto info = repo.GetInfo(user.email);;
    return info->id;

}



UserInfo CreateUser::createUser (std::string body) {
    auto json = userver::formats::json::FromString(body);
    UserInfo user;

    user.email = json["email"].As<std::string>();
    user.name = json["name"].As<std::string>();
    user.password_hash = "hardcoded_hash_123";

    return user;

}

}
