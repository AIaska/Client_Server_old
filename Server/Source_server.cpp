#include "../shared/socket_utils/socket_helper_wnd.h" 

int main()
{
    CServerSocketHelper socket_helper;

    socket_helper.Init();
    socket_helper.Listen();
    socket_helper.Accept();
    std::string res = "";

    for (int i = 0; i < 5; i++)
    {
        socket_helper.Receive(res);
        //std::cout << "res " << res << " length " << res.length() << '\n';
        socket_helper.Send(res, res.length());
    }

    // shutdown on command
    int i;
    std::cin >> i;
    socket_helper.Shutdown();
    
    return 0;
}
