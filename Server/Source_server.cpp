#include "../shared/socket_utils/socket_helper_wnd.h" 

int main()
{
    CServerSocketHelper socket_helper;

    socket_helper.Init();
    socket_helper.Listen();
    socket_helper.Accept();
    socket_helper.Receive();
    socket_helper.Shutdown();
    
    return 0;
}
