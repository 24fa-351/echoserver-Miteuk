#include "server.h"

void handle_client(int client_socket) {
    char buffer[MAX_BUFFER];
    int bytes_received;
    int total_bytes = 0;

    while (1) {
        memset(buffer, 0, MAX_BUFFER);
        total_bytes = 0;
        while (1) {
            bytes_received = recv(client_socket, &buffer[total_bytes], 1, 0);

            if (bytes_received <= 0) {
                break;  
            }

            total_bytes += bytes_received;

            if (buffer[total_bytes - 1] == '\n') {
                send(client_socket, buffer, total_bytes, 0);  // Echo the full line back
                break;
            }
        }

        if (bytes_received <= 0) {
            break;  
        }
    }

    close(client_socket);
}
// Function to start the server
void start_server(int port) {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // Create a socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Failed to create socket");
        exit(1);
    }

    // Set up the server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    // Bind the socket to the address and port
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Failed to bind socket");
        close(server_socket);
        exit(1);
    }

    // Start listening for incoming connections
    if (listen(server_socket, 1) == -1) {
        perror("Failed to listen on socket");
        close(server_socket);
        exit(1);
    }

    printf("Server is listening on port %d...\n", port);

    // Main loop to accept and handle client connections
    while (1) {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket == -1) {
            perror("Failed to accept client connection");
            continue;
        }

        printf("Client connected, handling communication...\n");

        handle_client(client_socket);

        printf("Client disconnected, waiting for a new connection...\n");
    }

    close(server_socket);
}