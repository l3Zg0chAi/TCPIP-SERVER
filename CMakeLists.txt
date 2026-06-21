cmake_minimum_required(VERSION 3.10)
project(tcpserver VERSION 1.0)
set(CMAKE_CXX_STANDARD 14)
include_directories(include)
include_directories(include/TCPCommunication)
include_directories(include/Common)
#include_directories(database/include)

set(SOURCES
    src/Application.cpp
)
set(TCPCOMMUNICATION_SOURCES
    src/TCPCommunication/TCPConnection.cpp
    src/TCPCommunication/TCPCommunicator.cpp
    src/TCPCommunication/TCPListener.cpp
    src/TCPCommunication/SendCombind.cpp
)
#set(DATABASE_SOURCES
#    database/src/DatabaseConnection.cpp
#    database/src/DatabaseManager.cpp
#)
add_executable(tcpserver 
                ${SOURCES}
                ${TCPCOMMUNICATION_SOURCES}
                #${DATABASE_SOURCES}
              )
target_link_libraries(tcpserver -pthread dlt mysqlcppconn) 
