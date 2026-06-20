#include "TCPConnection.h"
#include "Logger.h"
#include <TCPCommunicator.h>
#include <CommonFunction.h>

TCPConnection::TCPConnection(ClientConnInfo info) 
    : _info(info), _stopFlag(true), _isCanRemv(false), _state(ESTATE_CONNECTIONS::CLOSED)
{
    DEBUG_LOG ("Constructor TCPConnection objejct with ID %u", _info.connID);
}

TCPConnection::~TCPConnection()
{
    stop();
}

void TCPConnection::close_connection()
{
    if (_info.sockfd >= 0){
        shutdown(_info.sockfd, SHUT_RDWR);
        close(_info.sockfd);
        _info.sockfd = -1;
    }
}

void TCPConnection::stop()
{
    close_connection();
    _stopFlag.store(true);
    if (_rxThread.joinable()){
        _rxThread.join();
    }
    if (_txThread.joinable()){
        _txThread.join();
    }
}

bool TCPConnection::isCanRemovedConnection()
{
    return _isCanRemv.load();
}

void TCPConnection::setState(ESTATE_CONNECTIONS state)
{
    if (state != _state){
        _state = state;
    }
}

void TCPConnection::start()
{
    DEBUG_LOG("connection was established");
    if (!_stopFlag.load()){
        DEBUG_LOG("connection already running");
        return;
    }                                                        
    _stopFlag.store(false);
    setState(ESTATE_CONNECTIONS::CONNECTED);
    _rxThread = std::thread(&TCPConnection::rxWorker, this);
    _txThread = std::thread(&TCPConnection::txWorker, this);
}

bool TCPConnection::receive_from_client()
{
    Packet packet; // will receive data to here
    int ret = read_pdu(packet);
    if (ret < 0){
        if (ret == -2){
            DEBUG_LOG("CLOSE BY PEER");
        } else {
            DEBUG_LOG("CLOSE BY RST");
        }
        return false;
    }
    if ( ret > 0){
        DEBUG_LOG("read pdu success with %d bytes", ret);
    }

    TCPCommunicator::get_instance()->pushToRxQueue(std::move(packet));
    return true;
}

bool TCPConnection::send_to_client()
{
    Packet packet;
    while(_txQueue.wait_and_pop(packet)){
        int payload_length = packet.getPayloadLen();
        int total_length = 8 + payload_length;
        int byte = 0;
        while (byte < total_length){
            int ret = EthernetTimeSyncSend(_info.sockfd, packet._rawData.data()+byte,total_length-byte,2,0);
            if (ret < 0){
                return false;
            }
            byte += ret;
        }
        DEBUG_LOG("packet %u send %u byte, done", packet.getPDU(), byte);
    }
    return true;
}

int TCPConnection::write_pdu()
{
    return 0;
}

int TCPConnection::read_pdu(Packet& packet)
{
    packet._rawData.resize(8); // allocate 8byte to receive PDUID and payload length
    int result = 0;
    int byte = 0;

    for(int i = 0; i<4; i++){
        byte += result;
        result = EthernetTimeSyncRecv(_info.sockfd, packet._rawData.data() + byte, 1, 2, 0);
        if (result <= 0){
            DEBUG_LOG("It is impossible to not read at least 1 byte, TCP Connection maybe have a trouble");
            return result;
        }
    }

    byte += result;
    while (8 > byte){
        result = EthernetTimeSyncRecv(_info.sockfd, packet._rawData.data() + byte, 8 - byte, 2, 0);
         if (result < 0){
            return result;
        }
        byte += result;
    }

    int payload_length = packet.getPayloadLen();
    packet._rawData.resize(8+payload_length);

    byte = 0;
    while (byte < payload_length){
        result = EthernetTimeSyncRecv(_info.sockfd, packet._rawData.data() + byte + 8, payload_length - byte, 2, 0);
         if (result < 0){
            return result;
        }
        byte += result;
    }
    return result;
}

void TCPConnection::rxWorker()
{
    setCurrentThreadName("rxThread" + std::to_string(_info.connID));
    while(!_stopFlag.load()){
        switch(_state){
            case ESTATE_CONNECTIONS::CONNECTED:{
                if (!receive_from_client()){
                    setState(ESTATE_CONNECTIONS::CLOSED);
                }
                break;
            }
            case ESTATE_CONNECTIONS::CLOSED:{
                _isCanRemv.store(true);
                break;
            }
        }
    }
}

void TCPConnection::txWorker()
{
    setCurrentThreadName("txThread" + std::to_string(_info.connID));
    while(!_stopFlag.load()){
        switch(_state){
            case ESTATE_CONNECTIONS::CONNECTED:{
                if (!send_to_client()){
                    setState(ESTATE_CONNECTIONS::CLOSED);
                }
                break;
            }
            case ESTATE_CONNECTIONS::CLOSED:{
                _isCanRemv.store(true);
                break;
            }
        }
    }
}
