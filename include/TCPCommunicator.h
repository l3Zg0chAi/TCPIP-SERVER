#ifndef TCP_COMMUNICATOR
#define TCP_COMMUNICATOR

#include <unordered_map>
#include <memory>
#include "ThreadSafeQueue.h"
#include "CommonDef.h"
#include <atomic>

class TCPListenner;
class TCPConnection;

class TCPCommunicator {
public:
    static TCPCommunicator* get_instance(){
        static TCPCommunicator m_instance;
        return &m_instance;
    }

    TCPCommunicator();

    void start();
    bool onAcceptedClient(int clientfd);
    // void pushToQueue(Packet value);
    // bool receive_packet(Packet& value);

    
private:
    std::unordered_map<ListenInfo, std::unique_ptr<TCPListenner>> _listenners;
    std::unordered_map<ConnectionID, std::unique_ptr<TCPConnection>> _connections;
    std::atomic<UI_32> _connID;
    std::mutex _connectsMutex;
    // ThreadSafeQueue<Packet> _txQueueAllConn;
};

#endif // TCP_COMMUNICATOR