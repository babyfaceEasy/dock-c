// Defines our interface handler classes
#pragma once

#include <crow.h>
#include<crow/middlewares/cors.h>

using App = crow::App<crow::CORSHandler>;

class IHandler {
public:
    virtual void registerRoutes(App &app) = 0;

};