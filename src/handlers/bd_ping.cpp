#include "bd_ping.hpp"

#include <greeting.hpp>
#include <userver/storages/postgres/component.hpp>

namespace anxiety_backend {

PostgresPing::PostgresPing     
(   
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& component_context
)
    : HttpHandlerBase(config, component_context),
      pg_cluster_(component_context.FindComponent<userver::components::Postgres>("postgres-db-1").GetCluster())
{}

std::string PostgresPing::HandleRequestThrow(
    // Первый аргумент — HTTP-запрос.
    // Здесь он нам не нужен, потому что /db-ping не читает query params,
    // headers или body. Поэтому параметр оставлен без имени.
    const userver::server::http::HttpRequest&,

    // Второй аргумент — контекст запроса userver.
    // Он тоже не используется в этом handler-е, поэтому без имени.
    userver::server::request::RequestContext& 
) const {
    // const после сигнатуры метода означает:
    // этот метод не должен менять поля объекта PostgresPing.
    //
    // Например, он может использовать pg_cluster_,
    // но не должен переназначать его на другой cluster.

    // Выполняем SQL-запрос через PostgreSQL cluster.
    //
    // pg_cluster_ — это поле класса, которое мы заранее получили
    // в конструкторе через component_context.FindComponent(...).GetCluster().
    //
    // Оператор -> используется потому, что pg_cluster_ — это указатель-like объект.
    // То есть мы вызываем метод Execute у объекта, на который он указывает.
    const auto res = pg_cluster_->Execute(
        // ClusterHostType::kMaster означает:
        // выполнить запрос на master/primary базе.
        //
        // Для INSERT/UPDATE/DELETE всегда нужен master.
        // Для SELECT в production иногда используют replica,
        // но у нас локальная БД одна, поэтому kMaster нормально.
        userver::storages::postgres::ClusterHostType::kMaster,

        // Сам SQL-запрос.
        //
        // SELECT 1 не обращается к таблицам.
        // Он просто просит PostgreSQL вернуть число 1.
        //
        // Это удобно для проверки:
        // "жив ли коннект к БД и может ли backend выполнить SQL".
        "SELECT 1"
    );

    // res — это не само число 1.
    // res — это объект результата PostgreSQL-запроса.
    //
    // AsSingleRow<int>() говорит:
    // "ожидаю, что запрос вернул одну строку и одно значение,
    //  достань это значение как int".
    //
    // Для SELECT 1 результат как раз подходит:
    // одна строка, одна колонка, значение 1.
    auto result = res.AsSingleRow<int>();

    // Проверяем, что PostgreSQL вернул именно 1.
    //
    // Если result == 1, значит запрос выполнился корректно,
    // и связь backend → PostgreSQL работает.
    if (result == 1)
        return "ok";

    // Если PostgreSQL по какой-то причине вернул не 1,
    // возвращаем error.
    //
    // В реальности для SELECT 1 такая ветка почти никогда не должна случиться.
    // Если БД недоступна или SQL упал, userver обычно выбросит исключение раньше,
    // и клиент получит HTTP 500.
    return "error";
}

}

