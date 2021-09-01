#include "console_commands.h"

int main()
{
    try
    {
        CConsoleCommands consoleCmds;
        while (consoleCmds.Process()) continue;
    }
    catch (const exception& ex)
    {
        cout << __FUNCTION__ << "threw exception: " << ex.what() << '\n';
    }
    
    return 0;
}
