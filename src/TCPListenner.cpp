#include "TCPListenner.h"
#include "Logger.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "TCPCommunicator.h"

TCPListenner::TCPListenner(ListenInfo info, TCPCommunicator* ser_comm) :
    _listeninfo(info), _listenfd(-1), _servercomm(ser_comm)
{
    DEBUG_LOG("Listenner is created with ip %s port %d", _listeninfo.serverADDR.c_str(), _listeninfo.serverPort);
}

bool TCPListenner::open_listenner()
{
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
        DEBUG_LOG("bind ip %s and port %u server fail", _listenfd.serverADDR.c_str(), _listenfd.serverPort);
        return false;
    } else {
        DEBUG_LOG("bind ip %s and port %u server success", _listenfd.serverADDR.c_str(), _listenfd.serverPort);
    }

    if (listen(_listenfd, 1) == -1){
        DEBUG_LOG("bind fail errno=%d error=%s", errno, strerror(errno));
        return false;
    } else {
        DEBUG_LOG("Listenning on ip %s and port %u server...", _listenfd.serverADDR.c_str(), _listenfd.serverPort);
    }

    _acceptThread = std::thread(&TCPListenner::acceptWorker, this);
    return true;
}

void TCPListenner::acceptWorker()
{
    setCurrentThreadName("acceptThread" + std::to_string(_listeninfo.serverPort));
    struct sockaddr_in clientAddr{};
    socklen_t clientLen = sizeof(clientAddr);
    int client_fd = accept(_listenfd, (struct sockaddr*)&clientAddr, &clientLen);
    if(client_fd < 0){
        DEBUG_LOG("accept fail errno=%d error=%s", errno, strerror(errno));
        return;
    }

    // show infomation of client
    std::string clientIp = inet_ntoa(clientAddr.sin_addr);
    UI_16 clientPort = ntohs(clientAddr.sin_port);
    DEBUG_LOG("accepted client %s:%u", clientIp.c_str(), clientPort);

    _servercomm->onAcceptedClient(client_fd);
}
