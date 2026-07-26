#pragma once

#include "dto/user_dto.hpp"
#include "service/user_service.hpp"

#include <userver/components/component.hpp>
#include <userver/server/handlers/http_handler_base.hpp>

#include <userver/storages/postgres/cluster.hpp>

namespace anxiety_backend {

class CreateUserHandler : public userver::server::handlers::HttpHandlerBase {
    public:
        static constexpr std::string_view kName = "handler-create-user";

        CreateUserHandler(const userver::components::ComponentConfig&, const userver::components::ComponentContext&);

        std::string HandleRequestThrow(const userver::server::http::HttpRequest&, userver::server::request::RequestContext&)
            const override;
            
    private:
        const UserService& user_service_;
        UserDto Deserialize (std::string json) const;
        
};

}
