#pragma once
#include <models/user.hpp>
#include <userver/components/component.hpp>
#include <userver/server/handlers/http_handler_base.hpp>

#include <userver/storages/postgres/cluster.hpp>

namespace anxiety_backend {

class SetUser : public userver::server::handlers::HttpHandlerBase {
    public:
        static constexpr std::string_view kName = "handler-pg-set-user";

        SetUser(const userver::components::ComponentConfig&, const userver::components::ComponentContext&);

        std::string HandleRequestThrow(const userver::server::http::HttpRequest&, userver::server::request::RequestContext&)
            const override;
            
    private:
        userver::storages::postgres::ClusterPtr pg_cluster_;

        static UserInfo createUser (std::string json);
        void insertUser (UserInfo user) const;
        std::string getUserInfo (std::string email) const;
        
};

}
