#ifndef TCP_COMMUNICATOR
#define TCP_COMMUNICATOR

#include <unordered_map>
#include <memory>
#include "ThreadSafeQueue.h"
#include "CommonDef.h"
#include <atomic>

class TCPListener;
class TCPConnection;

class TCPCommunicator {
public:
    static TCPCommunicator* get_instance(){
        static TCPCommunicator m_instance;
        return &m_instance;
    }

    TCPCommunicator();
    ~TCPCommunicator();

    void start();
    bool onAcceptedClient(int clientfd, ListenID listenID);
    bool onRemovedClient();
    void pushToRxQueue(Packet&& packet);
    bool receive_packet(Packet& packet);
    void send_packet(ListenID lisenId, Packet packet);

    void getPDUData(PDUID id, Packet& packet);
private:
    std::unordered_map<ListenID, std::unique_ptr<TCPListener>> _listeners;
    std::unordered_map<ConnectionID, std::unique_ptr<TCPConnection>> _connections;
    std::atomic<UI_8> _connID;
    std::mutex _connectsMutex;
    std::mutex _removesMutex;
    ThreadSafeQueue<Packet> _rxQueueAllConn;   
};

#endif // TCP_COMMUNICATOR