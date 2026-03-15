#include "server.h"
#include "controller.h"

#include <crow.h>
#include <crow/middlewares/cors.h>
#include <iostream>

namespace server {

	void runServer() {
        crow::App<crow::CORSHandler> app;

        auto& cors = app.get_middleware<crow::CORSHandler>();
        cors.global()
            .headers("X-Custom-Header", "Upgrade-Insecure-Requests", "Content-Type, Authorization")
            .methods("POST"_method, "GET"_method, "OPTIONS"_method)
            .origin("*");

        controller::setupRoute(app);

        std::cout << "started server http://localhost:8080\n";

        app.port(8080).multithreaded().run();
	}

}