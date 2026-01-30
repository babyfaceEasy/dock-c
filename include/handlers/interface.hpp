// Defines our interface handler classes
#pragma once

#include <crow.h>

using App = crow::SimpleApp;

class IHandler {
public:
    virtual void registerRoutes(App &app) = 0;

}