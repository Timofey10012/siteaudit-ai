#include <crow.h>
#include <crow/middlewares/cors.h>
#include <iostream>

int main()
{
    crow::App<crow::CORSHandler> app;

    auto& cors = app.get_middleware<crow::CORSHandler>();
    cors.global()
        .headers("X-Custom-Header", "Upgrade-Insecure-Requests", "Content-Type, Authorization")
        .methods("POST"_method, "GET"_method, "OPTIONS"_method)
        .origin("http://localhost:5501")
        .origin("http://127.0.0.1:5501");

    CROW_ROUTE(app, "/ping").methods(crow::HTTPMethod::Get)(
        []() {
            crow::json::wvalue respJson;
            respJson["text"] = "pong";
            return respJson;
        });

    std::cout << "started server http://localhost:8080\n";

    app.port(8080).multithreaded().run();

    return 0;
}