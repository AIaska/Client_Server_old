#ifdef _WIN64
#include "socket_utils/socket_helper_wnd.h"
#endif

using namespace std;

int main(int argc, char** argv)
{
    try
    {
        if (argc != 2) {
            printf("usage: %s server-name\n", argv[0]);
            //return 1;
        }

        CClientSocketHelper socketHelper;

        socketHelper.Init("127.0.0.1"/*argv[2]*/);
        socketHelper.Connect();

        for (int i = 0; i < 5; i++)
        {
            socketHelper.Send("test msg");
            socketHelper.Receive();
        }

        socketHelper.Shutdown();
    }
    catch (const exception& ex)
    {
        cout << __FUNCTION__ << "threw exception: " << ex.what() << '\n';
    }

    return 0;
}
