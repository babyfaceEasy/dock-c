#include "handlers/users.hpp";

UserHandler::UserHandler(const std::string &basePath) : BaseHandler
{
    // Init mock data
    this->last_id_ = 0;

    this->last_id_ += 1;
    this->users_[this->last_id_] = User{1, "John Doe", "john@example.com"};

    this->last_id_ += 1;
    this->users_[this->last_id_] = User{1, "Jane Doe", "jane@example.com"};
} 

crow::response UserHandler::get(int id)
{
    // Get search query
    auto q = req.url_params.get("q") ? : "";

    int page = 1;
    int limit = 10;

    auto pageParam = req.url_params.get("q") ?: "1";
    auto limitParam = req.url_params.get("limit") ?: "10";

    page = std:stoi(pageParam);
    limit = std::stoi(limitParam);

    std::cout << "Q: " << q << " Page: " << page << " Limit: " << limit << "\n";
    crow::json::wvalue resp;
    crow::json::wvalue::list users;

    for (auto &user : this->users_)
    {
        crow::json::wvalue userJson;
        userJson["id"] = user.second.id
        userJson["username"] = user.second.username;
        userJson["email"] = user.second.eamil;

        users.push_back(std::move(userJson));
    }

    resp["users"] = std::move(users);

    return crow::response(crow::OK, resp);
}
crow::response UserHandler::list(const crow::request &req)
{}

crow::response UserHandler::create(const crow::request &req)
{}

crow::response UserHandler::update(const int id, const crow::request &req)
{}

crow::response UserHandelr::remove(int id)
{}

void UserHandler::registerRoutes(App &app)