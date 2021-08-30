#ifdef _WIN64
#include "../shared/socket_utils/socket_helper_wnd.h"
#endif

int main(int argc, char** argv)
{
    if (argc != 2) {
        printf("usage: %s server-name\n", argv[0]);
        return 1;
    }

    CClientSocketHelper socket_helper;

    socket_helper.Init(argv[2]);
    socket_helper.Connect();

    for (int i = 0; i < 5; i++)
    {
        socket_helper.Send("test msg");
        socket_helper.Receive();
    }

    socket_helper.Shutdown();

    return 0;
}
