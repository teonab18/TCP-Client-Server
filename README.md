# Multithreaded TCP Client-Server 

A concurrent TCP client-server application built in **C++** using the **Winsock2 API**. This project demonstrates low-level network programming, socket communication, and multithreading on Windows.

## 🚀 Features

* **Multithreading:** The server can handle multiple client connections simultaneously using the `<thread>` library.
* **Socket Programming:** Direct interaction with TCP/IP layers using Winsock.
* **Header Inspection:** Automatically extracts and displays packet details directly from the socket headers, including:
  * Source IP Address
  * Destination IP Address
  * Source Port
  * Destination Port
* **Robust Communication:** Graceful handling of client disconnections and continuous message listening.

## 🛠️ Technologies Used

* **Language:** C++
* **API:** Winsock2 (`ws2tcpip.h`, `winsock2.h`)
* **Libraries:** `<thread>`, `<iostream>`, `<string>`
* **Architecture:** Client-Server Model (TCP - Layer 4)

## ⚙️ How to Compile and Run

Because this project uses the Winsock library, you need to link the `ws2_32` library during compilation.

### Prerequisites
* A Windows environment.
* A C++ compiler (like MinGW/GCC) or Visual Studio.

### Compiling using MinGW (Terminal)
Open your terminal in the project folder and run:

```bash
# Compile the Server
g++ server.cpp -o server.exe -lws2_32

# Compile the Client
g++ client.cpp -o client.exe -lws2_32
