#include "handlers/users.hpp"
#include <nlohmann/json-schema.hpp>
#include <nlohmann/json.hpp>

using nlohmann::json;
using nlohmann::json_schema::json_validator;

// define the schema once
static json userSchema = R"({
    "$schema": "https://json-schema.org",
    "type": "object",
    "properties" : {
        "username": { "type": "string", "minLength": 3 },
        "email": { "type": "string", "format": "email"},
        "age": { "type": "integer", "minimum": 5},
    },
    "required": ["username", "email"]
})"_json;

UserHandler::UserHandler(const std::string &basePath) : BaseHandler(basePath)
{
    // Init mock data
    this->last_id_ = 0;

    this->last_id_ += 1;
    this->users_[this->last_id_] = User{1, "John Doe", "john@example.com"};

    this->last_id_ += 1;
    this->users_[this->last_id_] = User{1, "Jane Doe", "jane@example.com"};
}

crow::response UserHandler::create(const crow::request &req)
{
    try {
        // 1. Parse body
        json request_json = json::parse(req.body);
        
        // 2. Validate against schema
        json_validator validator;
        validator.set_root_schema(userSchema);
        validator.validate(request_json); // Throws if invalid

        // 3. Extract values
        std::string username = request_json["username"];
        std::string email = request_json["email"];

        // 4. Thread-safe data insertion (The missing part!)
        this->mutex_.lock();
        this->last_id_ += 1;
        User newUser{this->last_id_, username, email};
        this->users_[this->last_id_] = newUser;
        this->mutex_.unlock();

        // 5. Build Crow response
        crow::json::wvalue response;
        response["id"] = newUser.id;
        response["username"] = newUser.username;
        response["email"] = newUser.email;

        return crow::response(crow::CREATED, response);

    } catch (const std::exception &e) {
        crow::json::wvalue err_res;
        err_res["error"] = "Validation failed";
        err_res["message"] = e.what();
        return crow::response(crow::BAD_REQUEST, err_res);
    }
}


crow::response UserHandler::get(int id)
{
   if (this->users_.find(id) == this->users_.end()) 
   {
        return this->not_found("User not found");
   }
   User user = this->users_[id];
   crow::json::wvalue userJson;
   userJson["id"] = user.id;
   userJson["username"] = user.username;
   userJson["email"] = user.email;

   return crow::response(crow::OK, userJson);
}
crow::response UserHandler::list(const crow::request &req)
{
    // Get search query
    auto q = req.url_params.get("q") ? req.url_params.get("q") : "";

    int page = 1;
    int limit = 10;

    auto pageParam = req.url_params.get("page") ? req.url_params.get("page") : "1";
    auto limitParam = req.url_params.get("limit") ? req.url_params.get("limit") : "10";

    page = std::stoi(pageParam);
    limit = std::stoi(limitParam);

    std::cout << "Q: " << q << " Page: " << page << " Limit: " << limit << "\n";
    crow::json::wvalue resp;
    crow::json::wvalue::list users;

    for (auto &user : this->users_)
    {
        crow::json::wvalue userJson;
        userJson["id"] = user.second.id;
        userJson["username"] = user.second.username;
        userJson["email"] = user.second.email;

        users.push_back(std::move(userJson));
    }

    resp["users"] = std::move(users);

    return crow::response(crow::OK, resp);
}

crow::response UserHandler::update(const crow::request &req, const int id)
{

    // check to see if the user exists
    if (this->users_.find(id) == this->users_.end())
    {
        return this->not_found("User not found");
    }

    // get values from the request body
    crow::json::rvalue json = crow::json::load(req.body);

    std::string username = json["username"].s();
    std::string email = json["email"].s();

    this->mutex_.lock();

    User user = this->users_[id];
    user.username = username;
    user.email = email;

    this->users_[id] = user;

    this->mutex_.unlock();

    crow::json::wvalue response;
    response["id"] = user.id;
    response["username"] = user.username;
    response["email"] = user.email;

    return crow::response(crow::OK, response);

}

crow::response UserHandler::remove(int id)
{
    if(this->users_.find(id) == this->users_.end())
    {
        return this->not_found("User not found");
    }

    this->mutex_.lock();
    this->users_.erase(id);
    this->mutex_.unlock();

    crow::json::wvalue response;
    response["success"] = true;
    response["message"] = "User deleted successfully";

    return crow::response(crow::OK, response);
}

void UserHandler::registerRoutes(App &app)
{
    app.route_dynamic(this->basePath)
        .methods(crow::HTTPMethod::GET)(
            [this](const crow::request &req)
            {
                return this->list(req);
            }
        );
    
    app.route_dynamic(this->basePath + "/<int>")
        .methods(crow::HTTPMethod::GET)(
            [this](const crow::request &req, int id)
            {
                return this->get(id);
            }
        );
    
    app.route_dynamic(this->basePath)
        .methods(crow::HTTPMethod::POST)(
            [this](const crow::request &req)
            {
                return this->create(req);
            }
        );

    app.route_dynamic(this->basePath + "/<int>")
        .methods(crow::HTTPMethod::PUT)(
            [this](const crow::request &req, int id){
                return this->update(req, id);
        });

    app.route_dynamic(this->basePath + "/<int>")
        .methods(crow::HTTPMethod::DELETE)(
            [this](const crow::request &req, int id){
                return this->remove(id);
        });
}