#include "update_user_name.hpp"
#include "repositories/user_repository.hpp"

#include <userver/storages/postgres/component.hpp>

namespace anxiety_backend {

UpdateUserName::UpdateUserName   
(   
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& component_context
)
    : HttpHandlerBase(config, component_context),
      pg_cluster_(component_context.FindComponent<userver::components::Postgres>("postgres-db-1").GetCluster())
{}

std::string UpdateUserName::HandleRequestThrow(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext&  
)  const {
    
    auto json = userver::formats::json::FromString(request.RequestBody());
    auto email = json["email"].As<std::string>();
    auto name = json["name"].As<std::string>();

    auto &responseCode = request.GetHttpResponse();

     if (email.empty()){
        responseCode.SetStatus(userver::server::http::HttpStatus::kBadRequest);
        return "email is required";
    }

    UserRepository repo (pg_cluster_);
    auto flag = repo.UpdateName(email, name);

    if (!flag) {
        responseCode.SetStatus(userver::server::http::HttpStatus::NotFound);
        return R"({"error":"incorrect email"})";
    }
    auto info = repo.GetInfo(email);
    return info->id;

}
}
