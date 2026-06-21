#ifndef SEND_COMBIND
#define SEND_COMBIND

#include "Logger.h"
#include "CommonDef.h"
#include <condition_variable>
#include <atomic>
#include <chrono>
#include <vector>

struct CyclicTxPacket{
    CyclicTxPacket(){}
    CyclicTxPacket(
        ListenID id_,
        std::chrono::microseconds intervals_,
        std::chrono::steady_clock::time_point nextSendTime_
    ) : listenId(id_), intervals(intervals_), nextSendTime(nextSendTime_) {}

    ListenID listenId;
    std::chrono::microseconds intervals;
    std::chrono::steady_clock::time_point nextSendTime;
};

class SendCombind {
public:
    static SendCombind* get_instance(){
        static SendCombind m_instance;
        return &m_instance;
    }
    SendCombind();
    ~SendCombind();

    void init(); // init
    void start();
    void send_cyclic();

    void updateAllPacket();
    void trigger();
private:
    std::thread _cyclicThread;
    std::mutex _cyclicMutex;

    std::unordered_map<PDUID, Packet> packets;
    std::unordered_map<PDUID, CyclicTxPacket> tasks;
    std::condition_variable _cv;
    std::atomic<bool> _stopFlag;
};

#endif // SEND_COMBIND