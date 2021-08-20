#pragma once // TO DO can we use pragma or should we use ifdef-s ?

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
