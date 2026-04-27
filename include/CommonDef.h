#ifndef COMMON_DEF
#define COMMON_DEF
#include <iostream>
#include <cstdint>
#include <string>
#include <unordered_map>

typedef std::uint8_t UI_8;
typedef std::uint16_t UI_16;
typedef std::uint32_t UI_32;
typedef UI_8 ListenID;
typedef UI_8 ConnectionID;

#define ListenID_ONE 1U
#define ListenID_TWO 2U
#define ListenID_THREE 3U

#define SERVER_IP ""

struct ListenInfo {
    std::string serverADDR;
    UI_32 serverPort;
};

struct ClientConnInfo {
    UI_32 sockfd;
    ConnectionID connID;

    ClientConnInfo& operator=(const ClientConnInfo& other){
        sockfd = other.sockfd;
        connID = other.connID;
    }
};

struct Packet {

};

const std::unordered_map<ListenID, ListenInfo> ListennerInfoTable = {
    {ListenID_ONE,   {SERVER_IP, 10001}},
    {ListenID_TWO,   {SERVER_IP, 10002}},
    {ListenID_THREE, {SERVER_IP, 10003}}
}; 

#endif // COMMON_DEF