#pragma once

#include <crow.h>
#include <iostream>

struct CORSMiddleware
{
    struct context {};

    void before_handle(crow::request& req, crow::response& res, context& ctx)
    {
        res.add_header("Access-Control-Allow-Origin", "*");
        res.add_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        res.add_header("Access-Control-Allow-Headers", "Content-Type");
    }

    void after_handle(crow::request& req, crow::response& res, context& ctx)
    {
        res.add_header("Access-Control-Allow-Origin", "*");
    }
};