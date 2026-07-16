#pragma once

#include "models/user.hpp"
#include "service/user_service.hpp"

#include <userver/components/component.hpp>
#include <userver/server/handlers/http_handler_base.hpp>

#include <userver/storages/postgres/cluster.hpp>

namespace anxiety_backend {

class CreateUser : public userver::server::handlers::HttpHandlerBase {
    public:
        static constexpr std::string_view kName = "handler-pg-create-user";

        CreateUser(const userver::components::ComponentConfig&, const userver::components::ComponentContext&);

        std::string HandleRequestThrow(const userver::server::http::HttpRequest&, userver::server::request::RequestContext&)
            const override;
            
    private:
        UserService user_service_;
        static UserInfo createUser (std::string json);
        
};

}
