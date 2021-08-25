#ifdef _WIN64
#include "../Shared/Socket_utils/SocketHelper_wnd.h"
#endif

int main(int argc, char** argv)
{
    if (argc != 2) {
        printf("usage: %s server-name\n", argv[0]);
        return 1;
    }

    ClientSocketHelper socket_helper;

    socket_helper.Initialize(argv[2]);
    socket_helper.Connect();
    socket_helper.Send();
    socket_helper.Shutdown();
    socket_helper.Receive();

    return 0;
}
