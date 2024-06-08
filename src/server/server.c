// server.c

#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <mpi.h>

// Function to set up the server
int setup_server(void) {
    int server_socket;
    struct sockaddr_in server_addr;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Could not create socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 3) < 0) {
        perror("Listen failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server setup complete. Listening on port 8080\n");
    return server_socket;
}

// Function to accept a client connection
int accept_connection(int server_socket) {
    int client_socket;
    struct sockaddr_in client_addr;
    socklen_t addr_size = sizeof(client_addr);

    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_size);
    if (client_socket < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    return client_socket;
}

void* handle_client(void* client_socket) {
    // Implement request handling logic here
    // For simplicity, this example will assume fixed endpoints and operations

    int socket = *(int*)client_socket;
    char buffer[1024];
    read(socket, buffer, sizeof(buffer) - 1);

    if (strstr(buffer, "GET /read_files") != NULL) {
        handle_read_files("/path/to/directory", 0); // Adjust parameters as needed
    } else if (strstr(buffer, "GET /parse_xml") != NULL) {
        handle_parse_xml();
    }

    close(socket);
    return NULL;
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    // Server setup code
    int server_socket = setup_server();
    while (1) {
        int client_socket = accept_connection(server_socket);
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, (void*)&client_socket);
        pthread_detach(thread_id);
    }

    MPI_Finalize();
    return 0;
}
// server.c