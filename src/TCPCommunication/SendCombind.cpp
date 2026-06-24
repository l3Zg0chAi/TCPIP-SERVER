#include "SendCombind.h"
#include "TCPCommunicator.h"
SendCombind::SendCombind() : _stopFlag(true)
{
    init();
}

SendCombind::~SendCombind()
{
    _stopFlag.store(true);
    if (_cyclicThread.joinable()){
        _cyclicThread.join();
    }
}

void SendCombind::init()
{
    for(const auto& task : C_InfoTable_Packet){
        const InfoTable_packet& info = task.second;
        CyclicTxPacket cyclicPacket(info.listenId, std::chrono::microseconds(info.intervals), std::chrono::steady_clock::now());
        tasks[task.first] = std::move(cyclicPacket);

        Packet packet;
        packets[task.first] = std::move(packet);
    }
}

void SendCombind::start()
{
    if (!_stopFlag.load()){
        DEBUG_LOG("send cyclic already run");
    }
    _stopFlag.store(false);
    _cyclicThread = std::thread(&SendCombind::send_cyclic, this);
}

void SendCombind::send_cyclic()
{
    while(!_stopFlag){
        std::unique_lock<std::mutex> lock(_cyclicMutex);
        auto now = std::chrono::steady_clock::now();
        auto nextWakeup = now + std::chrono::hours(24);
        for(auto& task : tasks){
            PDUID pduid = task.first;
            ListenID listenid = task.second.listenId;
            CyclicTxPacket& cyclicPacket = task.second;
            if (now >= cyclicPacket.nextSendTime){
                TCPCommunicator::get_instance()->send_packet(listenid, packets[pduid]);
                cyclicPacket.nextSendTime = std::chrono::steady_clock::now() + cyclicPacket.intervals;
            }

            if (nextWakeup > cyclicPacket.nextSendTime){
                nextWakeup = cyclicPacket.nextSendTime;
            }
        }
        _cv.wait_until(lock, nextWakeup);
    }
}

void SendCombind::updateAllPacket()
{
    for(auto& packet : packets){
        PDUID pduid = packet.first;
        Packet& _packet = packet.second;
        TCPCommunicator::get_instance()->getPDUData(pduid, _packet);
    }
}

void SendCombind::trigger()
{
}
