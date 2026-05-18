#pragma once

#include "common/errors.h"

#include <string>
#include <optional>

namespace service {

	std::optional<errors::ErrorCode> analyze(const std::string& url, std::string& AIAnalyzeResult);

}