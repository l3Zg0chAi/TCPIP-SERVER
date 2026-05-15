#include "TCPListener.h"
#include "Logger.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "TCPCommunicator.h"

TCPListener::TCPListener(ListenInfo info, TCPCommunicator* ser_comm) :
    _listeninfo(info), _listenfd(-1), _servercomm(ser_comm), _stopFlag(true)
{
    DEBUG_LOG("Listener is created with ip %s port %d", _listeninfo.serverADDR.c_str(), _listeninfo.serverPort);
}

TCPListener::~TCPListener()
{
    stop();
}

void TCPListener::stop()
{
    if (_listenfd >= 0){
        shutdown(_listenfd, SHUT_RDWR);
        close(_listenfd);
        _listenfd = -1;
    }
    _stopFlag.store(true);;
    if (_acceptThread.joinable()){
        _acceptThread.join();
    }
}

bool TCPListener::open_listener()
{
    if (!_stopFlag.load()) {
        DEBUG_LOG("already open listener socket");
        return false;
    }
    _stopFlag.store(false);

    if ((_listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        DEBUG_LOG("bind fail errno=%d error=%s", errno, strerror(errno));
        return false;
    }

    // config ip port server mà sẽ dùng để listen
    struct sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(_listeninfo.serverPort);
    inet_aton(_listeninfo.serverADDR.c_str(), &serverAddr.sin_addr);

    // bind server addr and port to socket
    if (bind(_listenfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1){
        DEBUG_LOG("bind ip %s and port %u server fail", _listeninfo.serverADDR.c_str(), _listeninfo.serverPort);
        return false;
    } else {
        DEBUG_LOG("bind ip %s and port %u server success", _listeninfo.serverADDR.c_str(), _listeninfo.serverPort);
    }

    if (listen(_listenfd, 1) == -1){
        DEBUG_LOG("bind fail errno=%d error=%s", errno, strerror(errno));
        return false;
    } else {
        DEBUG_LOG("Listenning on ip %s and port %u server...", _listeninfo.serverADDR.c_str(), _listeninfo.serverPort);
    }

    _acceptThread = std::thread(&TCPListener::acceptWorker, this);
    return true;
}

void TCPListener::acceptWorker()
{
    setCurrentThreadName("act_Th " + std::to_string(_listeninfo.serverPort));
    struct sockaddr_in clientAddr{};
    socklen_t clientLen = sizeof(clientAddr);
    DEBUG_LOG("waiting client connect to ...");
    while (!_stopFlag.load()){
        int client_fd = accept(_listenfd, (struct sockaddr*)&clientAddr, &clientLen);
        if(client_fd < 0){
            DEBUG_LOG("accept fail errno=%d error=%s", errno, strerror(errno));
            if (_stopFlag.load()) break;
            continue;
        }

        // show infomation of client
        std::string clientIp = inet_ntoa(clientAddr.sin_addr);
        UI_16 clientPort = ntohs(clientAddr.sin_port);
        DEBUG_LOG("accepted client %s:%u", clientIp.c_str(), clientPort);

        _servercomm->onAcceptedClient(client_fd);
    }
}
