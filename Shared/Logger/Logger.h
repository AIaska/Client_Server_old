#pragma once

#include <iostream>
#include <string>

class Logger
{
public:
    static void to_console(std::string msg)
    {
        std::cout << msg << "\n";
    }
};
