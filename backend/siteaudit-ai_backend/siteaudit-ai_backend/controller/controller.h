#pragma once

#include <crow.h>
#include <crow/middlewares/cors.h>

namespace controller {

	void setupRoute(crow::App<crow::CORSHandler>& app);

}