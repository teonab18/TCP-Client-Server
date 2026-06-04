#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#pragma comment(lib, "ws2_32.lib")

using namespace std;
#define max_line 1024

//multithreading
void handle_client(SOCKET clientSocket, int client_id) {
    char line[max_line]; //mesajul
    while (true){
        memset(line, 0, max_line);
        int n = recv(clientSocket, line, max_line, 0); //ia mesajul + blocheaza thread crt pana cand vine un mesaj
        if (n <= 0){
            closesocket(clientSocket); //n=nr bytes
            break;
        } else {
            sockaddr_in peerAddr, localAddr;
            int pLen = sizeof(peerAddr), lLen = sizeof(localAddr);
            getpeername(clientSocket, (sockaddr*)&peerAddr, &pLen);//din header sursa
            getsockname(clientSocket, (sockaddr*)&localAddr, &lLen);//din header dest
            char ipSursa[INET_ADDRSTRLEN], ipDest[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &peerAddr.sin_addr, ipSursa, INET_ADDRSTRLEN);//ip binar in text
            inet_ntop(AF_INET, &localAddr.sin_addr, ipDest, INET_ADDRSTRLEN);
            cout << line
                 << " adresa IP sursa: " << ipSursa
                 << " adresa IP destinatie: " << ipDest
                 << " port sursa: " << ntohs(peerAddr.sin_port) //Network to Host Short
                 << " port destinatie: " << ntohs(localAddr.sin_port) << endl;
            send(clientSocket, line, n, 0);
        }
    }
}

int main(int argc, char** argv) {
    int listen_fd, conn_fd, server_port;
    cout << "Port server: " << endl;
    cin>>server_port;
    struct sockaddr_in cli_addr, serv_addr;
    WSADATA wsaData;
    if(WSAStartup(MAKEWORD(2,2), &wsaData)){
        cout << "Eroare";
        return 1;
    }
    listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(listen_fd == INVALID_SOCKET){
        WSACleanup();
        return 1;
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(server_port);
    int err = bind(listen_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (err == SOCKET_ERROR){
        WSACleanup();
        return 1; 
    }
    if(listen(listen_fd, SOMAXCONN) == SOCKET_ERROR){
        closesocket(listen_fd);
        WSACleanup();
        return 1;
    }
    int client_counter = 1;
    while (true) {
        int cli_len = sizeof(cli_addr);
        conn_fd = accept(listen_fd, (struct sockaddr*)&cli_addr, &cli_len);
        if (conn_fd != INVALID_SOCKET) {
            thread t(handle_client, conn_fd, client_counter);
            t.detach();
            
            client_counter++;
        }
    }
    closesocket(listen_fd);
    WSACleanup();
    return 0;
}