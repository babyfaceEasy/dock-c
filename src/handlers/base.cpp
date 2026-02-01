#include "handlers/base.hpp"

BaseHandler::BaseHandler(const std::string &basePath) : basePath_(basePath) {}

crow::response BaseHandler::bad_request(const std::string &message)
{
    crow::json::wvalue resp;

    resp["status"] = "error";
    resp["message"] = message;

    return crow::response(crow::BAD_REQUEST, resp);
}

crow::response BaseHandler::not_found(const std::string &message)
{
    crow::json::wvalue resp;

    resp["status"] = "error";
    resp["message"] = message;

    return crow::response(crow::NOT_FOUND, resp);
}

crow::response BaseHandler::internal(const std::string &message)
{
    crow::json::wvalue resp;

    resp["status"] = "error";
    resp["message"] = message;

    return crow::response(crow::INTERNAL_SERVER_ERROR, resp);
}



