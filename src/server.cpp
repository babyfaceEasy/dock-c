#include "server.hpp"

Server::Server(const ServerConfig &config) : config_(config)
{
    // set up logging level
    if (this->config_.log_level == "debug") 
    {
        crow::logger::setLogLevel(crow::LogLevel::DEBUG);
    } 
    else if (this->config_.log_level == "info")
    {
        crow::logger::setLogLevel(crow::LogLevel::INFO);
    }
    else if (this->config_.log_level == "warning")
    {
        crow::logger::setLogLevel(crow::LogLevel::WARNING);
    }
    else if (this->config_.log_level == "error")
    {
        crow::logger::setLogLevel(crow::LogLevel::ERROR);
    }
    else if (this->config_.log_level == "critical")
    {
        crow::logger::setLogLevel(crow::LogLevel::CRITICAL);
    }

    // App init
    this->app_ = std::make_unique<App>();

    // Config cors
    if (this->config_.cors)
    {
        auto &cors = this->app_->get_middleware<crow::CORSHandler>();

        cors
            .global()
            .methods(crow::HTTPMethod::GET, crow::HTTPMethod::POST, crow::HTTPMethod::PUT, crow::HTTPMethod::DELETE, crow::HTTPMethod::OPTIONS)
            .headers("Content-Type", "Authorization")
            .origin(this->config_.corsOrigin)
            .prefix("/api")
            .max_age(3600);
    }
   
}

void Server::setup()
{
    this->addHandlers(std::make_shared<UserHandler>("/api/users"));
}

void Server::addHandlers(std::shared_ptr<IHandler> handler)
{
    this->handlers_.push_back(handler);
    handler->registerRoutes(*this->app_);
}

void Server::start()
{
    this->setup();
    this->app_->port(this->config_.port).multithreaded().concurrency(this->config_.threads).run_async();
}