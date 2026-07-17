#pragma once

#include <userver/components/component.hpp>
#include <userver/server/handlers/http_handler_base.hpp>

#include <userver/storages/postgres/cluster.hpp>

#include "service/user_service.hpp"

namespace anxiety_backend {

class UpdateUserNameHandler : public userver::server::handlers::HttpHandlerBase {
    public:
        static constexpr std::string_view kName = "handler-pg-update-user-name";

        UpdateUserNameHandler(const userver::components::ComponentConfig&, const userver::components::ComponentContext&);

        std::string HandleRequestThrow(const userver::server::http::HttpRequest&, userver::server::request::RequestContext&)
            const override;
            
    private:
        const UserService& user_service_;
};

}
