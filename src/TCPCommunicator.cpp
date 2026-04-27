#include "TCPCommunicator.h"
#include "Logger.h"
#include "TCPListenner.h"
#include "TCPConnection.h"

TCPCommunicator::TCPCommunicator()
{
    setCurrentThreadName("TCPCommunicator");
    DEBUG_LOG("create all listenner connections object");
    for (const auto& listenner : ListennerInfoTable){
        _listenners[listenner.first] = std::make_unique<TCPListenner>(listenner.second, this);
    }

    _connID.store(0);
}

TCPCommunicator::~TCPCommunicator()
{
}

void TCPCommunicator::start()
{
    for (const auto& listenner : _listenners){
        listenner.second->open_listenner();
    }
}

bool TCPCommunicator::onAcceptedClient(int clientfd)
{
    DEBUG_LOG("on accept client connection");
    _connID.fetch_add(1);
    auto clientConn = std::make_unique<TCPConnection>(ClientConnInfo(clientfd, _connID));
    TCPConnection* rawConn = clientConn.get();
    {
        std::lock_guard<std::mutex> lockCon(_connectsMutex);
        _connections[_connID.load()] = std::move(clientConn);
    }

    rawConn->start();
    return true;
}

// void TCPCommunicator::pushToQueue(Packet value)
// {
//     _rxQueueAllConn.push(value);
// }

// bool TCPCommunicator::receive_packet(Packet &value)
// {
//     if (_rxQueueAllConn.try_pop(value)){
//         return true;
//     }
//     return false;
// }
