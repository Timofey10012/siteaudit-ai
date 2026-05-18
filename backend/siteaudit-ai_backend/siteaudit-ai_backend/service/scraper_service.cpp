#include "service/scraper_service.h"

#include <cstdio>
#include <memory>
#include <array>
#include <format>

namespace scraper_service {

	namespace {
		std::string runScraper(const std::string& url) {
			std::string command = std::format("node scraper/scraper.js \"{}\"", url);

			std::array<char, 4096> buffer;
			std::string result;

			FILE* pipe = _popen(command.c_str(), "r");

			if (!pipe) {
				std::cout << "Failed to run scraper\n";
				return result;
			}

			while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
				result += buffer.data();
			}

			_pclose(pipe);
			return result;
		}
	}

	std::optional<errors::ErrorCode> scraper(const std::string& url, std::string& pageContent) {

		std::string raw = runScraper(url);

		auto json = crow::json::load(raw);

		if (!json) return errors::ErrorCode::InternalError;

		if (json["status"] == "error") {
			std::string msg = json["message"].s();
			std::cout << msg << "\n";
			return errors::ErrorCode::BadGateway;
		}
		
		if (json["content"].t() == crow::json::type::String) {
			pageContent = json["content"].s();
		}
		else {
			std::cout << "Data is not string\n";
			return errors::ErrorCode::InternalError;
		}

		return std::nullopt;
	}

}