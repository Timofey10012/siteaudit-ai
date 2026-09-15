#pragma warning(disable : 4996)

#include "service/ai_service.h"

#include <format>
#include <cstdlib>
#include <iostream>
#include <curl/curl.h>

namespace ai_service {

	namespace {
		std::string escapeJson(const std::string& input) {
			std::string output;
			output.reserve(input.size());

			for (char c : input) {
				switch (c) {
				case '\"': output += "\\\""; break;
				case '\\': output += "\\\\"; break;
				case '\b': output += "\\b"; break;
				case '\f': output += "\\f"; break;
				case '\n': output += "\\n"; break;
				case '\r': output += "\\r"; break;
				case '\t': output += "\\t"; break;
				default:
					output += c;
				}
			}

			return output;
		}

		std::string createdPromt(const std::string& pageContent) {
			std::string promt = std::format(
				"You are a UX and SEO analyst.\n\n"

				"Analyze the provided website content.\n"
				"The content is a structured extraction from the DOM (not a full visual render), so some elements may appear incomplete or out of context.\n\n"

				"Website data:\n"
				"{}\n\n"

				"Your tasks:\n"
				"1. Briefly describe what the website is about.\n"
				"2. Identify 3–5 REAL problems in UX, structure, clarity, or content.\n"
				"3. Suggest specific, actionable improvements.\n"
				"4. Mention strengths.\n\n"

				"IMPORTANT ANALYSIS RULES:\n"
				"- Do NOT assume something is missing if links or sections suggest it exists.\n"
				"- If information is present but requires scrolling or navigation, treat it as a UX issue (not absence).\n"
				"- Distinguish between:\n"
				"  • 'missing information'\n"
				"  • 'poorly structured or hard to find information'\n"
				"- Avoid false claims.\n"
				"- Base conclusions ONLY on provided data.\n\n"

				"UX FOCUS:\n"
				"- Navigation clarity\n"
				"- Information hierarchy\n"
				"- Content readability\n"
				"- Accessibility of key actions (apply, pricing, programs)\n\n"

				"SEO FOCUS:\n"
				"- Heading clarity and meaning\n"
				"- Keyword relevance (generic vs specific headings)\n"
				"- Content structure\n\n"

				"STYLE RULES:\n"
				"- Be specific and practical\n"
				"- Avoid generic phrases\n"
				"- Keep it concise\n"
				"- Prioritize real user experience problems over technical HTML issues\n"
				"- Focus on what affects user behavior and conversion\n\n"

				"OUTPUT REQUIREMENTS:\n"
				"- Return ONLY valid JSON.\n"
				"- NO markdown formatting (do not use ```json ... ```).\n"
				"- NO explanations outside JSON.\n"
				"- JSON KEYS MUST BE IN ENGLISH (camelCase).\n"
				"- ALL STRING VALUES (content inside the keys) MUST BE WRITTEN IN RUSSIAN.\n\n"

				"JSON SCHEMA (keys in English, values in Russian):\n"
				"{{\n"
				"  \"version\": \"2.0\",\n"
				"  \"pageType\": \"Тип страницы (например: Лендинг, Интернет-магазин, Блог)\",\n"
				"  \"overallScore\": 7, // число от 1 до 10\n"
				"  \"shortDescription\": \"Краткое описание сайта на русском языке\",\n"
				"  \"issues\": [\n"
				"    {{\n"
				"      \"title\": \"Короткий заголовок проблемы на русском\",\n"
				"      \"description\": \"Подробное описание проблемы на русском языке\",\n"
				"      \"severity\": \"high\" | \"medium\" | \"low\"\n"
				"    }}\n"
				"  ],\n"
				"  \"improvements\": [\n"
				"    \"Конкретный совет по улучшению на русском языке\"\n"
				"  ],\n"
				"  \"strengths\": [\n"
				"    \"Описание сильной стороны на русском языке\"\n"
				"  ]\n"
				"}}\n"
				, pageContent);

			return escapeJson(promt);
		}

		size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
			size_t totalSize = size * nmemb;
			output->append((char*)contents, totalSize);
			return totalSize;
		}

		bool sendRequest(const std::string& prompt, std::string& response) {
			const char* apiKey = std::getenv("ROUTERAI_API_KEY");
			if (!apiKey) {
				std::cout << "API key not found\n";
				return false;
			}

			CURL* curl = curl_easy_init();
			
			std::string jsonBody = R"({
				"model": "deepseek/deepseek-v3.2",
				"messages": [
					{
						"role": "user",
						"content": ")" + prompt + R"("
					}
				]
			})";
			
			struct curl_slist* headers = NULL;
			headers = curl_slist_append(headers, ("Authorization: Bearer " + std::string(apiKey)).c_str());
			headers = curl_slist_append(headers, "Content-Type: application/json");
			
			curl_easy_setopt(curl, CURLOPT_URL, "https://routerai.ru/api/v1/chat/completions");
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonBody.c_str());
			
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

			CURLcode res = curl_easy_perform(curl);
			
			if (res != CURLE_OK) {
				std::cout << curl_easy_strerror(res) << "\n";
				return false;
			}
			
			curl_easy_cleanup(curl);
			
			curl_slist_free_all(headers);
			
			return true;
		}

	}

	bool runAIAnalyze(const std::string& pageContent, std::string& AIAnalyzeResult) {
		std::string promt = createdPromt(pageContent);
		
		if (!sendRequest(promt, AIAnalyzeResult)) return false;

		return true;
	}

}