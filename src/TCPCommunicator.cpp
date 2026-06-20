#include "TCPCommunicator.h"
#include "Logger.h"
#include "TCPListener.h"
#include "TCPConnection.h"

TCPCommunicator::TCPCommunicator()
{
    setCurrentThreadName("TCPCommunicator");
    DEBUG_LOG("create all listener connections object");
    for (const auto& listener : ListenerInfoTable){
        _listeners[listener.first] = std::make_unique<TCPListener>(listener.second, this);
    }

    _connID.store(0);
}

TCPCommunicator::~TCPCommunicator()
{
}

void TCPCommunicator::start()
{
    for (const auto& listener : _listeners){
        listener.second->open_listener();
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
        DEBUG_LOG("size of _connections %d", _connections.size());
    }

    rawConn->start();
    return true;
}

bool TCPCommunicator::onRemovedClient()
{
    // use this - dont lock when call destructor TCPConnection, dont call stop and join thread in mutex lock
    std::vector<std::unique_ptr<TCPConnection>> removed;
    std::lock_guard<std::mutex> lock_rem(_removesMutex);
    for (auto it = _connections.begin(); it != _connections.end(); ) {
        if (it->second->isCanRemovedConnection()) {
            DEBUG_LOG("erase conn id=%u", it->first);

            removed.push_back(std::move(it->second));
            it = _connections.erase(it);
        } else {
            ++it;
        }
    }
    return true;
}

void TCPCommunicator::pushToRxQueue(Packet&& value)
{
    _rxQueueAllConn.push(std::move(value));
}

bool TCPCommunicator::receive_packet(Packet &value)
{
    if (_rxQueueAllConn.try_pop(value)){
        return true;
    }
    return false;
}
