#include "Application.h"
#include "TCPCommunicator.h"
#include "Logger.h"
#include <thread>
#include <chrono>

DltContext main_dltCxt; // define context

void Application::init()
{
    TCPCommunicator::get_instance()->start();
}

void Application::execute()
{
    while (true){
        DEBUG_LOG("start execute");
        receive_from_client();

        TCPCommunicator::get_instance()->onRemovedClient();
        DEBUG_LOG("end execute");
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void Application::receive_from_client()
{
    // Packet packet;
    // while(TCPCommunicator::get_instance()->receive_packet(packet)){
    //     // handle data here
    // }
    // DEBUG_LOG("not element in queue any more");
}

int main() {
    DLT_REGISTER_APP("STCP", "TCPServer Application"); // register app with DLT Daemon
    DLT_REGISTER_CONTEXT(main_dltCxt, "MAIN", "Main application context"); // register context of app with DLT Daemon

    IDBConnection* dbcon = new DBConnection;
    if (!dbcon->initialize("tcp://192.168.4.114:3306", "hailn", "hailn", "DBFrom_hailn")){
        return -1;
    }

    IDBManager* dbmanager = new DBManager(dbcon);
    dbmanager->initialize();

    Application::get_instance()->init();
    Application::get_instance()->execute();

    dbcon->disconnect();
    delete dbcon;
    delete dbmanager;
    
    DLT_UNREGISTER_CONTEXT(main_dltCxt); // unregister context
    DLT_UNREGISTER_APP(); // unregister app
    return 0;
}