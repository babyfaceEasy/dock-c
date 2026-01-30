#pragma once


#include <string>
#include "interface.hpp"

class BaseHandler : public IHandler 
{
public: 
    explicit BaseHandler(const std::string &basePath);

private:
    std::string basePath;

    // Utility functions
    static crow::response bad_request(const std::string &message);
    static crow::response not_found(const std::string &message);
    static crow::response internal(const std::string &message);

}