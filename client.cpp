#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation failed" << std::endl;
        return 1;
    }

    // Set up address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 address from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        return 1;
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        return 1;
    }

    // Receive welcome message from server
    char buffer[1024] = {0};
    read(sock, buffer, 1024);
    std::cout << buffer << std::endl;

    // Send symptoms to server
    int symptom;
    std::cout << "Enter your symptom (1 for Fever, 2 for Cold, 3 for Cough): ";
    std::cin >> symptom;
    send(sock, &symptom, sizeof(int), 0);

    // Receive diagnosis from server
    read(sock, buffer, 1024);
    std::cout << "Diagnosis: " << buffer << std::endl;

    // Close socket
    close(sock);
     return 0;
}   
