#include <crow.h>
#include <iostream>

int main()
{
    crow::App<> app;

    CROW_ROUTE(app, "/ping")(
        []() {
            crow::json::wvalue respJson;
            respJson["text"] = "pong";
            return respJson;
        });

    std::cout << "started server http://localhost:8080\n";

    app.port(8080).multithreaded().run();

    return 0;
}