#include "Application.h"
#include "TCPCommunicator.h"
#include "Logger.h"
#include <thread>
#include <chrono>
#include <SendCombind.h>

DltContext main_dltCxt; // define context

void Application::init()
{
    TCPCommunicator::get_instance()->start();
    SendCombind::get_instance()->start();
}

void Application::execute()
{
    while (true){
        DEBUG_LOG("start execute");
        handleFromClient();
        TCPCommunicator::get_instance()->onRemovedClient();

        SendCombind::get_instance()->updateAllPacket();
        DEBUG_LOG("end execute");
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void Application::handleFromClient()
{
    Packet packet;
    while(TCPCommunicator::get_instance()->receive_packet(packet)){
        UI_32 pdu = packet.getPDU();
        
        switch(pdu){
            case CONST_PDU_TCP0101:
            break;
            case CONST_PDU_TCP0102:
            break;
            case CONST_PDU_TCP0103:
            break;
            case CONST_PDU_TCP0104:
            default:
            break;
        }
    }
}

int main() {
    DLT_REGISTER_APP("STCP", "TCPServer Application"); // register app with DLT Daemon
    DLT_REGISTER_CONTEXT(main_dltCxt, "MAIN", "Main application context"); // register context of app with DLT Daemon

    // IDBConnection* dbcon = new DBConnection;
    // if (!dbcon->initialize("tcp://192.168.4.114:3306", "hailn", "hailn", "DBFrom_hailn")){
    //     return -1;
    // }

    // IDBManager* dbmanager = new DBManager(dbcon);
    // dbmanager->initialize();

    Application::get_instance()->init();
    Application::get_instance()->execute();

    // dbcon->disconnect();
    // delete dbcon;
    // delete dbmanager;
    
    DLT_UNREGISTER_CONTEXT(main_dltCxt); // unregister context
    DLT_UNREGISTER_APP(); // unregister app
    return 0;
}