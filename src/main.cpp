#include <userver/clients/dns/component.hpp>
#include <userver/clients/http/component_list.hpp>
#include <userver/components/component.hpp>
#include <userver/components/component_list.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/congestion_control/component.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/testsuite/testsuite_support.hpp>



#include <userver/storages/postgres/component.hpp> 

#include <userver/utils/daemon_run.hpp>

#include <hello.hpp>
#include <hello_postgres.hpp> 
#include <handlers/bd_ping.hpp>
#include <handlers/user_crud/get_user.hpp>
#include <handlers/user_crud/create_user.hpp>
#include <handlers/user_crud/delete_user.hpp>
#include <handlers/user_crud/update_user_name.hpp>
#include <handlers/auth/login.hpp>
#include <handlers/auth/register.hpp>

int main(int argc, char* argv[]) {
    auto component_list =
        userver::components::MinimalServerComponentList()
            .Append<userver::server::handlers::Ping>()
            .Append<userver::components::TestsuiteSupport>()
            .AppendComponentList(userver::clients::http::ComponentList())
            .Append<userver::clients::dns::Component>()
            .Append<userver::server::handlers::TestsControl>()
            .Append<userver::congestion_control::Component>()
            .Append<anxiety_backend::Hello>()
            .Append<anxiety_backend::JwtService>()
            .Append<anxiety_backend::UserService>()
            .Append<anxiety_backend::AuthService>()
            .Append<userver::components::Postgres>("postgres-db-1")
            .Append<anxiety_backend::HelloPostgres>()
            .Append<anxiety_backend::PostgresPing>()
            .Append<anxiety_backend::GetUserHandler>()
            .Append<anxiety_backend::CreateUserHandler>()
            .Append<anxiety_backend::DeleteUserHandler>()
            .Append<anxiety_backend::UpdateUserNameHandler>()
            .Append<anxiety_backend::LogInUserHandler>()
            .Append<anxiety_backend::RegisterHandler>()
            
        ;

    return userver::utils::DaemonMain(argc, argv, component_list);
}

