#include <crow.h>
#include <crow/middlewares/cors.h>
#include <iostream>


int main()
{
    crow::App<crow::CORSHandler> app;

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