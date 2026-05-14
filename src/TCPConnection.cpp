#include "TCPConnection.h"
#include "Logger.h"

TCPConnection::TCPConnection(ClientConnInfo info) 
    : _info(info), _stopFlag(true), _state(ESTATE_CONNECTIONS::CLOSED)
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
    _stopFlag = true;
    if (_rxThread.joinable()){
        _rxThread.join();
    }
    if (_txThread.joinable()){
        _txThread.join();
    }
}

bool TCPConnection::isStopped()
{
    return _stopFlag;
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
    if (!_stopFlag){
        DEBUG_LOG("connection already running");
        return;
    }                                                        
    _stopFlag = false;
    _rxThread = std::thread(&TCPConnection::rxWorker, this);
    _txThread = std::thread(&TCPConnection::txWorker, this);
}

bool TCPConnection::receive_from_client()
{
    int ret = read_pdu();
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
    return true;
}

int TCPConnection::read_pdu()
{
    char buffer[1024];
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(_info.sockfd, &readfds);
    struct timeval timeout;
    timeout.tv_sec = 2;   // 2 second
    timeout.tv_usec = 0;
    int ret = select(_info.sockfd + 1, &readfds, nullptr, nullptr, &timeout);
    if (ret == 0){
        DEBUG_LOG("no data comes");
    }
    else if (ret < 0){
        DEBUG_LOG("select fail errno=%d error=%s", errno, strerror(errno));
    } 
    else{
        ret = recv(_info.sockfd, buffer, sizeof(buffer), 0);
        if ( ret > 0){
            DEBUG_LOG("recv %d bytes", ret);
        }
        else if (ret == 0){
            // close by peer, client close
            DEBUG_LOG("close by peer mean client close socket connection");
            ret == -2; // FIN
        }
        else {
            DEBUG_LOG("recv fail errno=%d error=%s", errno, strerror(errno));
        }
    }
    return ret;
}

void TCPConnection::rxWorker()
{
    setCurrentThreadName("rxThread" + std::to_string(_info.connID));
    while(!_stopFlag){
        switch(_state){
            case ESTATE_CONNECTIONS::CONNECTED:{
                if (!receive_from_client()){
                    setState(ESTATE_CONNECTIONS::CLOSED);
                }
                break;
            }
            case ESTATE_CONNECTIONS::CLOSED:{
                _stopFlag =  true;
                break;
            }
        }
    }
}

void TCPConnection::txWorker()
{
    setCurrentThreadName("txThread" + std::to_string(_info.connID));
    while(!_stopFlag){
        
    }
}
