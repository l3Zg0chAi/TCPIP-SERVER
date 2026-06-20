#ifndef COMMON_DEF
#define COMMON_DEF
#include <iostream>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <arpa/inet.h>
#include <vector>
typedef std::uint8_t UI_8;
typedef std::uint16_t UI_16;
typedef std::uint32_t UI_32;
typedef UI_8 ListenID;
typedef UI_8 ConnectionID;

#define ListenID_ONE 1U
#define ListenID_TWO 2U
#define ListenID_THREE 3U

#define SERVER_IP "192.168.24.128"

struct ListenInfo {
    std::string serverADDR;
    UI_32 serverPort;
};

struct ClientConnInfo {
    int sockfd;
    ConnectionID connID;
    ClientConnInfo(int sock, ConnectionID ID) : sockfd(sock), connID(ID){}

    ClientConnInfo& operator=(const ClientConnInfo& other){
        sockfd = other.sockfd;
        connID = other.connID;
        return *this;
    }
};

const std::unordered_map<ListenID, ListenInfo> ListenerInfoTable = {
    {ListenID_ONE,   {SERVER_IP, 10001}},
    {ListenID_TWO,   {SERVER_IP, 10002}},
    {ListenID_THREE, {SERVER_IP, 10003}}
}; 

struct Packet { 
    Packet() {}; 
    Packet(const std::vector<UI_8>& data) : _rawData(data) {} 

    std::vector<UI_8> _rawData; 
    UI_32 getPDU() const { 
        if (_rawData.size() < 8) { 
            return 0; // Invalid packet 
        } 
        return htonl(*(UI_32*)(&_rawData[0])); 
    } 
    UI_32 getPayloadLen() const{ 
        if(_rawData.size() < 8) { 
            return 0; // Invalid packet 
        } 
        return htonl(*(UI_32*)(&_rawData[4])); 
    } 
    bool operator == (const Packet& other) const { 
        return _rawData == other._rawData; 
    } 
    bool operator != (const Packet& other) const { 
        return _rawData != other._rawData;  
    }
};

struct TCP001{
    UI_32 PDUID;
    UI_32 payloadLength;
    
    UI_8 DATE;
    UI_8 MONTH;
    UI_8 YEAR;
};

#endif // COMMON_DEF