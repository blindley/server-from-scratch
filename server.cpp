// Server side C program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

struct Socket {
    int server_fd;
    sockaddr_in address;

    Socket()
    {
        this->server_fd = 0;
        memset(&this->address, 0, sizeof(this->address));
    }

    bool bind(unsigned short port)
    {
        int temp_server_fd;
        if ((temp_server_fd = ::socket(AF_INET, SOCK_STREAM, 0)) == 0) {
            perror("error in socket");
            return false;
        }

        sockaddr_in temp_address;

        temp_address.sin_family = AF_INET;
        temp_address.sin_addr.s_addr = INADDR_ANY;
        temp_address.sin_port = htons(port);

        memset(temp_address.sin_zero, 0, sizeof(address.sin_zero));
        
        if (::bind(temp_server_fd, (sockaddr*) &temp_address, sizeof(temp_address)) < 0) {
            perror("error in bind");
            close(temp_server_fd);
        }

        if (::listen(temp_server_fd, 10) < 0) {
            perror("error in listen");
            close(temp_server_fd);
            return false;
        }

        this->server_fd = temp_server_fd;
        this->address = temp_address;
        return true;
    }
};

#define PORT 8080
int main(int argc, char const *argv[])
{
    int server_fd, new_socket; long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    char const* hello = "Hello from server";

    Socket mySocket;
    if (!mySocket.bind(PORT)) {
        perror("error initializing Socket");
        exit(EXIT_FAILURE);
    }
    
    while(1)
    {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        int addrlen = sizeof(mySocket.address);
        int newSocket = accept(mySocket.server_fd, (struct sockaddr *)&mySocket.address,
            (socklen_t*)&addrlen);
        
        if (newSocket < 0) {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        
        char buffer[30000] = {0};
        valread = read(newSocket, buffer, 30000);
        printf("%s\n",buffer );
        write(newSocket , hello , strlen(hello));
        printf("------------------Hello message sent-------------------\n");
        close(newSocket);
    }
    return 0;
}
