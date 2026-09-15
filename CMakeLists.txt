cmake_minimum_required(VERSION 3.21)
project(ai_ux_auditor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES
    backend/*.cpp
)

add_executable(ai_ux_auditor_backend ${SOURCES})

add_custom_command(
    TARGET ai_ux_auditor_backend POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/scraper
        $<TARGET_FILE_DIR:ai_ux_auditor_backend>/scraper
    COMMAND cd $<TARGET_FILE_DIR:ai_ux_auditor_backend>/scraper && 
        if not exist node_modules (npm install --production)
    COMMAND cd $<TARGET_FILE_DIR:ai_ux_auditor_backend>/scraper && 
        if not exist "%LocalAppData%\\ms-playwright" (npx playwright install chromium)
)

target_include_directories(ai_ux_auditor_backend PRIVATE
    ${CMAKE_SOURCE_DIR}/backend/siteaudit-ai_backend/siteaudit-ai_backend
    ${CMAKE_SOURCE_DIR}/scraper
)

find_package(Crow CONFIG REQUIRED)
find_package(curl CONFIG REQUIRED)

target_link_libraries(ai_ux_auditor_backend PRIVATE
    Crow::Crow
    CURL::libcurl
)

if (WIN32)
    target_compile_definitions(ai_ux_auditor_backend PRIVATE
        _WIN32_WINNT=0x0A00
        WINVER=0x0A00
        _CRT_SECURE_NO_WARNINGS
    )
endif()