#ifndef TCP_LISTENNER
#define TCP_LISTENNER
#include "CommonDef.h"
#include <thread>
#include <mutex>
class TCPCommunicator;

class TCPListenner{
public:
    TCPListenner(ListenInfo info, TCPCommunicator* sercommun);
    bool open_listenner();

    void acceptWorker();
private:
    int _listenfd;
    ListenInfo _listeninfo;

    std::thread _acceptThread;
    std::mutex _acceptMtx;
    TCPCommunicator* _servercomm;
};

#endif // TCP_LISTENNER