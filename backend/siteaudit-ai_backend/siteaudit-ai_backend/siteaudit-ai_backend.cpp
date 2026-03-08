#include "cors.h"

#include <crow.h>
#include <iostream>

int main()
{
    crow::App<> app;

    CROW_ROUTE(app, "/<path>").methods(crow::HTTPMethod::Options)(
        [](std::string path) {
            return crow::response(200);
        });

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