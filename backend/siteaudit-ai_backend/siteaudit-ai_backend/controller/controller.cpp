#include "controller/controller.h"
#include "service/analyze_service.h"
#include "common/errors.h"

#include <curl/curl.h>
#include <regex>
#include <string>

namespace controller {

    namespace {
        bool isValidUrl(const crow::json::rvalue& bodyJson) {
            if (bodyJson["url"].t() == crow::json::type::Null) return false;

            const std::string url = bodyJson["url"].s();

            return curl_url_set(curl_url(), CURLUPART_URL, url.c_str(), 0) == CURLUE_OK;
        }

        std::string cleanLLMResponse(const std::string& raw) {
            std::string result = raw;

            result = std::regex_replace(result, std::regex(R"(^```json\s*)"), "");
            result = std::regex_replace(result, std::regex(R"(\s*```$)"), "");
            result = std::regex_replace(result, std::regex(R"(^\s+|\s+$)"), "");

            return result;
        }

    }

	void setupRoute(crow::App<crow::CORSHandler>& app) {
        CROW_ROUTE(app, "/analyze").methods(crow::HTTPMethod::Post)(
            [](const crow::request& req) {
                auto bodyJson = crow::json::load(req.body);

                if (!isValidUrl(bodyJson)) return errors::makeError(errors::ErrorCode::ValidationError);

                std::string url = bodyJson["url"].s();

                std::string AIAnalyzeResult;

                if (auto err = service::analyze(url, AIAnalyzeResult)) {
                    return errors::makeError(*err);
                }

                auto jsonAIAnalyzeResult = crow::json::load(AIAnalyzeResult);

                crow::json::wvalue respJson;

                respJson["analyze"] = cleanLLMResponse(jsonAIAnalyzeResult["choices"][0]["message"]["content"].s());

                return crow::response{ respJson };
            });
	}

}