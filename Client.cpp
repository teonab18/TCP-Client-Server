#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
using namespace std;

int main(){
    WSADATA wsaData;
    int server_port;
    cout << "port server: " << endl;
    cin >> server_port;
    cin.ignore();
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    char hostname[256];
    gethostname(hostname, sizeof(hostname)); //ip ul laptop de pe placa de retea wifi
    struct hostent* hostInfo = gethostbyname(hostname);
    char* serverIP = inet_ntoa(*(struct in_addr*)hostInfo->h_addr_list[0]);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0); //AF_INET=layer 4 IP, SOCK_STREAM=TCP layer 4
    if(sockfd == INVALID_SOCKET){
        cout << "Eroare" << endl;
        WSACleanup();
        return 1;
    }
    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(server_port);
    inet_pton(AF_INET, serverIP, &serv_addr.sin_addr);
    if(connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR){ //Windows aloca automat port dinamic
        cout << "Eroare conexiune server" << endl;
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }
    char buffer[1024];
    string mesaj;
    
    while(true){
        getline(cin, mesaj);
        if(mesaj == "exit")
            break;
        if(mesaj.empty())
            continue;
        send(sockfd, mesaj.c_str(), mesaj.length(), 0);
        memset(buffer, 0, 1024);
        int n = recv(sockfd, buffer, 1024, 0);
        if(n > 0){
            sockaddr_in peerAddr, localAddr;
            int pLen = sizeof(peerAddr), lLen = sizeof(localAddr);
            getpeername(sockfd, (sockaddr*)&peerAddr, &pLen);
            getsockname(sockfd, (sockaddr*)&localAddr, &lLen);
            char ipSursa[INET_ADDRSTRLEN], ipDest[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &peerAddr.sin_addr, ipSursa, INET_ADDRSTRLEN);
            inet_ntop(AF_INET, &localAddr.sin_addr, ipDest, INET_ADDRSTRLEN);
            cout << buffer 
                 << " adresa ip sursa: " << ipSursa 
                 << " adresa ip destinatie: " << ipDest 
                 << " adresa port sursa: " << ntohs(peerAddr.sin_port) 
                 << " adresa port destinatie:" << ntohs(localAddr.sin_port) << endl;
        }else{
            cout << "Conexiune server pierduta" << endl;
            break;
        }
    }

    closesocket(sockfd);
    WSACleanup();
    return 0;
}