#include "SocketClient.h"

SocketClient::SocketClient(const std::string& ip, int port) {
#ifdef _WIN32
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        throw std::runtime_error("WSAStartup failed");
    }
#endif

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        throw std::runtime_error("Socket creation failed");
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
#ifdef _WIN32
    InetPton(AF_INET, ip.c_str(), &serverAddr.sin_addr);
#else
    inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr);
#endif

    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        throw std::runtime_error("Connection to server failed");
    }

    std::cout << "[DEBUG] Conectado al servidor en " << ip << ":" << port << std::endl;
}

SocketClient::~SocketClient() {
#ifdef _WIN32
    closesocket(sock);
    WSACleanup();
#else
    close(sock);
#endif
}

std::string SocketClient::sendCommand(const std::string& command) {
    std::string cmdWithNewline = command + "\n";
    send(sock, cmdWithNewline.c_str(), cmdWithNewline.size(), 0);

    char buffer[1024] = {0};
    ssize_t bytesRead = recv(sock, buffer, sizeof(buffer) - 1, 0);

    if (bytesRead <= 0) {
        throw std::runtime_error("Disconnected or failed to read response");
    }

    buffer[bytesRead] = '\0';
    return std::string(buffer);
}