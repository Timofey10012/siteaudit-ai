#include "service/analyze_service.h"
#include "service/scraper_service.h"
#include "service/ai_service.h"

namespace service {

	std::optional<errors::ErrorCode> analyze(const std::string& url, std::string& AIAnalyzeResult) {

		std::string pageContent;

		std::cout << "Start scraper!\n";
		if (auto err = scraper_service::scraper(url, pageContent)) {
			return *err;
		}

		std::cout << "Start AI analyze!\n";
		if (!ai_service::runAIAnalyze(pageContent, AIAnalyzeResult)) {
			return errors::ErrorCode::InternalError;
		}

		return std::nullopt;
	}

}