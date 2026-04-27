cmake_minimum_required(VERSION 3.10)
project(tcpserver VERSION 1.0)
set(CMAKE_CXX_STANDARD 14)
include_directories(include)
set(SOURCES
    src/Application.cpp
    src/TCPConnection.cpp
    src/TCPCommunicator.cpp
    src/TCPListener.cpp
)
add_executable(tcpserver ${SOURCES})
target_link_libraries(tcpserver -pthread)