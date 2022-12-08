#include "socket.h"

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <string>
#include <optional>

auto string_to_int(char const* str) -> std::optional<int>
{
    try {
        auto n = std::stoi(str);
        return n;
    } catch (...) {
        return std::nullopt;
    }
}

#define DEFAULT_PORT 8080
int main1(int argc, char** argv)
{
    int port = DEFAULT_PORT;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0) {
            i++;
            if (i >= argc) {
                fprintf(stderr, "missing port number\n");
                return 1;
            }
            auto opt_port = string_to_int(argv[i]);
            if (!opt_port.has_value()) {
                fprintf(stderr, "invalid port number (%s)\n", argv[i]);
                return 1;
            }
            port = *opt_port;
        }
    }

    auto port_string = std::to_string(port);

    printf("Configuring local address...\n");
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    struct addrinfo* bind_address;
    getaddrinfo(0, port_string.c_str(), &hints, &bind_address);

    printf("Creating socket...\n");
    Socket socket_listen;
    socket_listen.socket = socket(bind_address->ai_family,
        bind_address->ai_socktype, bind_address->ai_protocol);

    if (!socket_listen.isValid()) {
        fprintf(stderr, "socket() failed. (%d)\n", Socket::getLastError());
        return 1;
    }

    printf("Binding socket to local address...\n");
    if (bind(socket_listen.socket, bind_address->ai_addr, (int)bind_address->ai_addrlen)) {
        fprintf(stderr, "bind() failed. (%d)\n", Socket::getLastError());
        return 1;
    }
    freeaddrinfo(bind_address);

    printf("Listening...\n");
    if (listen(socket_listen.socket, 10) < 0) {
        fprintf(stderr, "listen() failed. (%d)\n", Socket::getLastError());
        return 1;
    }

    printf("Waiting for connection...\n");
    struct sockaddr_storage client_address;
    socklen_t client_len = sizeof(client_address);
    Socket socket_client;
    socket_client.socket =
        accept(socket_listen.socket, (struct sockaddr*) &client_address, &client_len);
    if (!socket_client.isValid()) {
        fprintf(stderr, "accept() failed. (%d)\n", Socket::getLastError());
        return 1;
    }

    printf("Client is connected... ");
    char address_buffer[100];
    getnameinfo((struct sockaddr*) &client_address, client_len, address_buffer,
        sizeof(address_buffer), 0, 0, NI_NUMERICHOST);
    printf("%s\n", address_buffer);

    printf("Reading request...\n");
    char request[1024];
    int bytes_received = recv(socket_client.socket, request, 1024, 0);
    printf("Received %d bytes.\n", bytes_received);

    printf("============================ Received request ============================\n");
    printf("%.*s", bytes_received, request);
    printf("============================  End of request  ============================\n");

    printf("Sending response...\n");
    char const* response =
        "HTTP/1.1 200 OK\r\n"
        "Connection: close\r\n"
        "Content-Type: text/plain\r\n\r\n"
        "Local time is: ";
    int bytes_sent = send(socket_client.socket, response, (int)strlen(response), 0);
    printf("Sent %d of %d bytes.\n", bytes_sent, (int)strlen(response));

    time_t timer;
    time(&timer);
    char const* time_msg = ctime(&timer);
    bytes_sent = send(socket_client.socket, time_msg, (int)strlen(time_msg), 0);
    printf("Sent %d of %d bytes.\n", bytes_sent, (int)strlen(time_msg));

    printf("Closing connection...\n");
    socket_client.close();

    printf("Closing listening socket...\n");
    socket_listen.close();

    printf("Finished.\n");
    
    return 0;
}

int main(int argc, char** argv) {
    if (!Socket::init()) {
        fprintf(stderr, "Failed to initialize socket API!\n");
        return 1;
    }

    int result = main1(argc, argv);

    Socket::deInit();
    return result;
}
