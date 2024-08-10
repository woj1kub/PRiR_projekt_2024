#include "../headers/backup.h"
#include <fstream>
#define WIN32_LEAN_AND_MEAN
#define ShowCursor ShowCursorRaylib
#define CloseWindow CloseWindowRaylib
#define Rectangle RectangleRaylib
#include <WinSock2.h>
#include <iostream>


void backup() 
{
    std::string textToSave;

    textToSave.append(std::to_string(points));
    textToSave.append("\n");
    textToSave.append(std::to_string(seed));
    textToSave.append("\n");

    for (size_t i = 0; i < map.size(); ++i) {
        if (map[i].hasBuilding())
        {
            textToSave.append(map[i].toString());
            
        }
    }
    sendToserver(textToSave);
}

int sendToserver(string text)
{
    WSAData wsa;
    SOCKET s;
    sockaddr_in server;

    // Inicjalizacja Winsock
    std::cout << "Initializing Winsock..." << std::endl;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cerr << "Failed. Error Code: " << WSAGetLastError() << std::endl;
        return 1;
    }

    std::cout << "Initialized." << std::endl;

    // Tworzenie socketu
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        std::cerr << "Could not create socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    std::cout << "Socket created." << std::endl;

    // Konfiguracja serwera
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); // IP serwera
    server.sin_family = AF_INET;
    server.sin_port = htons(12345); // Port serwera

    // Połączenie do serwera
    if (connect(s, (struct sockaddr*)&server, sizeof(server)) < 0) {
        std::cerr << "Connect error: " << WSAGetLastError() << std::endl;
        closesocket(s);
        WSACleanup();
        return 1;
    }

    std::cout << "Connected to server." << std::endl;

    // Wiadomość do wysłania
    std::string message = text;
    if (send(s, message.c_str(), message.length(), 0) < 0) {
        std::cerr << "Send failed: " << WSAGetLastError() << std::endl;
        closesocket(s);
        WSACleanup();
        return 1;
    }

    std::cout << "Message sent." << std::endl;

    // Odbieranie odpowiedzi od serwera (opcjonalnie)
    char server_reply[1024];
    int recv_size = recv(s, server_reply, sizeof(server_reply), 0);
    if (recv_size == SOCKET_ERROR) {
        std::cerr << "Receive failed: " << WSAGetLastError() << std::endl;
    } else {
        server_reply[recv_size] = '\0';
        std::cout << "Server reply: " << server_reply << std::endl;
    }

    // Zamknięcie połączenia
    closesocket(s);
    WSACleanup();
    return 0;
}
