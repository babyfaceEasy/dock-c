#pragma once

#include <crow.h>
#include <memory>
#include <vector>
#include <string>


#include "handlers/interface.hpp"

struct ServerConfig {
    int port = 8080;
    int threads = 2;
    std::string log_level = "info";
    bool cors = true;
    std:string corsOrigin = "*"; 

}

class Server
{
public:
    explicit Server(const ServerConfig &config = ServerConfig());

    void start();

private:
    ServerConfig config_;
    std::unique_ptr<App> app_;
    std::vector<std::shared_ptr<IHandler>> handlers_;
    // Setup our handlers
    void setup();
    void addHandlers(std::shared_ptr<IHandler> handler);
}