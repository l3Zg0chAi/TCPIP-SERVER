#ifndef TCP_LISTENER
#define TCP_LISTENER
#include "CommonDef.h"
#include <thread>
#include <mutex>
#include <atomic>

class TCPCommunicator;

class TCPListener{
public:
    TCPListener(ListenInfo info, TCPCommunicator* sercommun);
    ~TCPListener();
    bool open_listener();
    void stop();
    void acceptWorker();
private:
    int _listenfd;
    ListenInfo _listeninfo;
    std::atomic<bool> _stopFlag;

    std::thread _acceptThread;
    std::mutex _acceptMtx;
    TCPCommunicator* _servercomm;
};

#endif // TCP_LISTENER