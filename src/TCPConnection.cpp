#include "TCPConnection.h"
#include "Logger.h"

TCPConnection::TCPConnection(ClientConnInfo info) 
    : _info(info), _stopFlag(true), _state(ESTATE_CONNECTIONS::CLOSED)
{
    DEBUG_LOG ("Constructor TCPConnection objejct");
}

TCPConnection::~TCPConnection()
{
    stop();
}

bool TCPConnection::receive_from_client()
{
    // nhận từ recv, push vào queue
    return false;
}

void TCPConnection::close_connection()
{
    if (_info.sockfd != -1){
        close(_info.sockfd);
        _info.sockfd = -1;
    }
}

void TCPConnection::stop()
{
    close_connection();
    _stopFlag = true;
    if (_rxThread.joinable()){
        _rxThread.join();
    }
}

void TCPConnection::start()
{
    DEBUG_LOG("connection was established");
    if (!_stopFlag){
        DEBUG_LOG("connection already running");
        return;
    }                                                        
    _stopFlag = false;
    _rxThread = std::thread(&TCPConnection::rxWorker, this);
    _txThread = std::thread(&TCPConnection::txWorker, this);
}

void TCPConnection::setState(ESTATE_CONNECTIONS state)
{
    if (state != _state){
        _state = state;
    }
}

void TCPConnection::rxWorker()
{
    setCurrentThreadName("rxThread" + std::to_string(_info.connID));
}

void TCPConnection::txWorker()
{
    setCurrentThreadName("txThread" + std::to_string(_info.connID));

}

void TCPConnection::config_optional_socket()
{
}
