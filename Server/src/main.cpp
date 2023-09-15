#include <iostream>
#include <cstring>
#include <mutex>
#include <queue>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <thread>
#include <unistd.h>
#include <arpa/inet.h>
#include "TrafficLight.h"

int handleClient(int client_socket) {
    close(client_socket);
    return 0;
}


struct Message {
    int clientId;
    std::string content;
};

void trafficLightController(std::mutex& mutex, std::queue<Message>& messageQueue) {
    while(true) {
        // Check if there is a queued cycle to execute
        // It not, check the message queue
        std::unique_lock<std::mutex> lock(mutex);
        if (messageQueue.empty()) {
            // No queued cycles or messaged, continue with current cycle.
            lock.unlock();
            //Implement traffic logic
        }

        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
}

int main() {
    std::queue<Message> messageQueue;
    std::mutex mutex;

    TrafficLight NB = TrafficLight();
    TrafficLight SB = TrafficLight();
    TrafficLight EB = TrafficLight();
    TrafficLight WB = TrafficLight();

    const char* server_ip = "127.0.0.1";
    const int server_port = 12345;
    const int MAX_CLIENTS = 4;

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        std::cerr << "Error creating server socket" << std::endl;
        return 1;
    }

    // Define server address struct
    // Last two lines convert our 
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(server_ip);
    server_address.sin_port = htons(server_port);

    // Bind socket to string-to-binary converted address and port above
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        std::cerr << "Error binding server socket" << std::endl;
        close(server_socket);
        return 1;
    }

    // Listen for incoming connections
    if (listen(server_socket, 4) == -1) {
        std::cerr << "Error listening on server socket" << std::endl;
        close(server_socket);
        return 1;
    }

    std::cout << "Server is listening on " << server_ip << ":" << server_port << std::endl;

    // Start a thread for the traffic light controller
    std::thread controllerThread(trafficLightController);

    while(true) {
        struct sockaddr_in client_address;
        socklen_t client_address_len = sizeof(client_address);
        int client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_len);
        if (client_socket == -1) {
            std::cerr << "Error accepting client connection" << std::endl;
            continue;
        }

        // Create a new thread to handle the client
        std::thread(handleClient, client_socket).detach();
    }
}