#include "SendCombind.h"

SendCombind::SendCombind() : _stopFlag(true)
{
    init();
}

SendCombind::~SendCombind()
{
}

void SendCombind::init()
{
    for(const auto& task : C_InfoTable_Packet){
        PDUID pduid = task.first;
        const InfoTable_packet& info = task.second;

        Packet packet;
        CyclicTxPacket cyclicPacket(packet, std::chrono::microseconds(info.intervals), std::chrono::steady_clock::now());
        tasks[task.first] = std::move(cyclicPacket);
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
        if (_stopFlag){
            return;
        }

        auto now = std::chrono::steady_clock()::now();
        for(auto& task : tasks){
            PDUID pduid = task.first;
            CyclicTxPacket& cyclicPacket = task.second;
            if (now >= cyclicPacket.nextSendTime){
                
                cyclicPacket.nextSendTime = now + cyclicPacket.intervals;
            }
        }
    }
}

void SendCombind::updateAllPacket()
{

}

void SendCombind::trigger()
{
}
