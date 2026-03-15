#pragma once

#include <crow.h>

namespace errors {

	enum class ErrorCode {
		ValidationError,
		InternalError
	};

	inline crow::response makeError(ErrorCode code) {
		int httpStatus;
		crow::json::wvalue json;

		if (code == ErrorCode::ValidationError) {
			httpStatus = 400;
			json["error"]["code"] = "VALIDATION_ERROR";
		}
		else {
			httpStatus = 500;
			json["error"]["code"] = "INTERNAL_ERROR";
		}

		return crow::response(httpStatus, json);
	}

}