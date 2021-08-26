#ifndef _LOGGER_H_INCLUDED_
#define _LOGGER_H_INCLUDED_


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

#endif // _LOGGER_H_INCLUDED_
