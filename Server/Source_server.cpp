#include "../Shared/Socket_utils/SocketHelper.h" 

int main()
{
    ServerSocketHelper socket_helper;

    socket_helper.Initialize();
    socket_helper.Listen();
    socket_helper.Accept();
    socket_helper.Receive();
    socket_helper.Shutdown();
    
    return 0;
}
