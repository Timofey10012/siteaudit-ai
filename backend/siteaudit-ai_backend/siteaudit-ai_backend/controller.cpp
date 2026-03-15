#include "controller.h"
#include "errors.h"

#include <regex>
#include <string>

namespace controller {

    namespace {
        bool isValidUrl(const crow::json::rvalue& bodyJson) {
            if (bodyJson["url"].t() == crow::json::type::Null) return false;

            const std::string url = bodyJson["url"].s();

            const std::regex url_regex(R"(^(https?://)?([\w-]+(.[\w-]+)+)([\w-,@?^=%&:/~+#]*[\w-,@?^=%&/~+#])?$)",
                std::regex::icase);

            return std::regex_match(url, url_regex);
        }
    }

	void setupRoute(crow::App<crow::CORSHandler>& app) {
        CROW_ROUTE(app, "/analyze").methods(crow::HTTPMethod::Post)(
            [](const crow::request& req) {
                auto bodyJson = crow::json::load(req.body);

                if (!isValidUrl(bodyJson)) return errors::makeError(errors::ErrorCode::ValidationError);

                crow::json::wvalue respJson;
                return respJson;
            });
	}

}