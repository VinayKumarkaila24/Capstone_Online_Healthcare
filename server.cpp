#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    char buffer[1024] = {0};
    std::string message = "Welcome to online healthcare services! Please enter your symptoms:\n1. Fever\n2. Cold\n3. Cough\n";

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "Socket creation failed" << std::endl;
        return 1;
    }

    // Set up address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind socket to address
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        return 1;
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        std::cerr << "Listen failed" << std::endl;
        return 1;
    }

    std::cout << "Server connected. Waiting for patients..." << std::endl;

    while (true) {
        // Accept incoming connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0) {
            std::cerr << "Accept failed" << std::endl;
            continue;
        }

        std::cout << "Patient connected." << std::endl;

        // Send welcome message to client
        send(new_socket, message.c_str(), message.length(), 0);

        // Receive symptoms from client
        int symptom;
        read(new_socket, &symptom, sizeof(int));

        // Generate diagnosis based on symptoms
        std::string diagnosis;
        if (symptom == 1) {
            diagnosis = "You have a Fever. Take the following medicines:\n1. Paracetamol\n2. Ibuprofen\n3. Aspirin";
        } else if (symptom == 2) {
            diagnosis = "You have a Cold. Take the following medicines:\n1. Antihistamines\n2. Decongestants\n3. Expectorants";
        } else if (symptom == 3) {
            diagnosis = "You have a Cough. Take the following medicines:\n1. Cough Syrup\n2. Expectorants\n3. Bronchodilators";
        } else {
            diagnosis = "Unknown symptom.";
        }

        // Send diagnosis to client
        send(new_socket, diagnosis.c_str(), diagnosis.length(), 0);

        // Close socket
        close(new_socket);
    }

    return 0;
}
