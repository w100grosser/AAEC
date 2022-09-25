#pragma once

#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <windows.h>


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

class packet_sender {
public:
    void init(PCSTR PORT);
    int send_packet(char* data, int length);
    int close_connection();
private: 

    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    char* sendbuf;
    struct addrinfo* result = NULL,
        * ptr = NULL,
        hints;

    int iResult;
    char recvbuf[88200];
    int recvbuflen = 88200;

};