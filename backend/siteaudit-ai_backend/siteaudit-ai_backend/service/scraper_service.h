#pragma once

#include "common/errors.h"

#include <string>
#include <optional>

namespace scraper_service {

	std::optional<errors::ErrorCode> scraper(const std::string& url, std::string& pageContent);

}