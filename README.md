# UDP Chat Application  

A lightweight **multi-threaded chat application** built in **C++** using **UDP sockets**. The project consists of a client and a server implementation (`client.cpp` and `server.cpp`), enabling real-time text-based communication over a network.  

## Features  
- Built with **C++ and POSIX sockets** for low-level network communication.  
- **Multi-threaded design** to handle sending and receiving messages concurrently.  
- Uses **UDP protocol** for fast, connectionless message transfer.  
- Synchronization with **mutexes** to avoid race conditions.  
- Simple **CLI interface** for ease of use.  

## Files  
- `server.cpp` → Server program that listens for incoming messages and broadcasts them to clients.  
- `client.cpp` → Client program that connects to the server and enables chat communication.  

## How to Build  

```bash
# Clone the repository
git clone https://github.com/AbhiAnand-1011/UDP-Chat.git
cd UDP-Chat

# Compile the server
g++ server.cpp -o server -pthread

# Compile the client
g++ client.cpp -o client -pthread
