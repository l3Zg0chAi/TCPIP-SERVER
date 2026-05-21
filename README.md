# TCPIP-SERVER

A lightweight multi-threaded TCP server written in C++.

This project creates a TCP server application that listens on multiple ports, accepts client connections, receives packet data, and pushes received packets into a thread-safe queue for further processing.

## Features

- TCP server implementation using POSIX sockets
- Multi-port listener support
- Multi-threaded client handling
- Separate RX/TX worker threads for each connection
- Thread-safe receive queue
- Basic connection lifecycle management
- DLT-based logging support
- CMake-based build system

## Tech Stack

- C++14
- CMake
- POSIX socket API
- pthread
- DLT logging library

## Project Structure

```text
TCPIP-SERVER/
├── include/
│   ├── Application.h
│   ├── CommonDef.h
│   ├── CommonFunction.h
│   ├── Logger.h
│   ├── TCPCommunicator.h
│   ├── TCPConnection.h
│   ├── TCPListener.h
│   └── ThreadSafeQueue.h
├── src/
│   ├── Application.cpp
│   ├── TCPCommunicator.cpp
│   ├── TCPConnection.cpp
│   └── TCPListener.cpp
├── CMakeLists.txt
├── rebuild.sh
└── .gitignore
```

## How It Works

The application starts from `src/Application.cpp`.

1. The application registers a DLT app and logging context.
2. `Application::init()` starts the TCP communicator.
3. `TCPCommunicator` creates TCP listeners based on `ListenerInfoTable`.
4. Each `TCPListener` opens a socket, binds to an IP/port, listens for clients, and accepts incoming connections.
5. When a client connects, `TCPCommunicator` creates a `TCPConnection`.
6. Each connection starts RX and TX worker threads.
7. The RX worker reads packets from the client and pushes them into the global receive queue.

## Server Configuration

The server IP and ports are configured in `include/CommonDef.h`.

```cpp
#define SERVER_IP "192.168.24.128"

const std::unordered_map<ListenID, ListenInfo> ListenerInfoTable = {
    {ListenID_ONE,   {SERVER_IP, 10001}},
    {ListenID_TWO,   {SERVER_IP, 10002}},
    {ListenID_THREE, {SERVER_IP, 10003}}
};
```

By default, the server listens on:

- `192.168.24.128:10001`
- `192.168.24.128:10002`
- `192.168.24.128:10003`

Update `SERVER_IP` and the port values to match your local environment before running the server.

## Packet Format

The server expects each packet to use the following binary format:

```text
+----------------+----------------+------------------+
| PDU ID         | Payload Length | Payload          |
| 4 bytes        | 4 bytes        | N bytes          |
+----------------+----------------+------------------+
```

The first 8 bytes are the packet header:

- First 4 bytes: PDU ID
- Next 4 bytes: payload length
- Remaining bytes: payload data

## Requirements

Before building the project, make sure the following tools/libraries are installed:

- C++ compiler with C++14 support
- CMake 3.10 or newer
- pthread
- DLT development library

On Ubuntu/Debian-based systems, you may need packages similar to:

```bash
sudo apt update
sudo apt install build-essential cmake
```

If your system does not already include the DLT library, install the corresponding DLT development package for your Linux distribution.

## Build

You can build the project using the provided script:

```bash
chmod +x rebuild.sh
./rebuild.sh
```

The script removes the old `build/` directory, recreates it, runs CMake, and builds the executable.

Alternatively, build manually:

```bash
mkdir -p build
cmake -S . -B build
cmake --build build
```

## Run

After building, run the server executable:

```bash
./build/tcpserver
```

Make sure the configured server IP exists on your machine. If the bind step fails, update `SERVER_IP` in `include/CommonDef.h`.

## Logging

This project uses DLT logging macros. The application registers:

```cpp
DLT_REGISTER_APP("STCP", "TCPServer Application");
DLT_REGISTER_CONTEXT(main_dltCxt, "MAIN", "Main application context");
```

Make sure your DLT daemon/environment is configured correctly if you want to view logs.

## Main Components

### Application

Responsible for initializing and executing the server application.

### TCPCommunicator

Manages all TCP listeners and active client connections. It also owns the shared receive queue.

### TCPListener

Creates the server socket, binds to the configured IP/port, listens for incoming clients, and accepts new connections.

### TCPConnection

Handles an accepted client connection. It manages the socket lifecycle and runs RX/TX worker threads.

### ThreadSafeQueue

Provides a thread-safe queue used to store packets received from client connections.

## Notes

- The TX worker is currently present but does not send data yet.
- Packet handling in `Application::receive_from_client()` is currently commented out.
- Update the server IP before running on another machine.
- This project is intended for learning or extending a basic TCP server architecture.

## License

No license has been provided yet. Add a license file if you plan to distribute or open-source this project.
