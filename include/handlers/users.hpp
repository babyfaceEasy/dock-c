#pragma once

#include <unordered_map>
#include <mutex>

#include "base.hpp"

class UserHandler : BaseHandler
{
public:
    UserHandler(const std::string &basePath);
    void registerRoutes(App &app);

private:
    crow::response list(const crow::request &req);
    crow::response get(int id);
    crow::response create(const crow::request &req);
    crow::response update(const int id, const crow::request &req);
    crow::response remove(int id);

    // mock data
    std::unordered_map<int, User> users_;
    int last_id_;
    std::mutex mutex_;

};