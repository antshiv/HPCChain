// server.h

#ifndef SERVER_H
#define SERVER_H

// Function to set up the server
int setup_server(void);

// Function to accept a client connection
int accept_connection(int server_socket);

#endif // SERVER_H
