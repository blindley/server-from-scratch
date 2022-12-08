#include "socket.h"

#include <stdio.h>


int main()
{
    if (!Socket::init()) {
        fprintf(stderr, "Failed to initialize socket API!\n");
        return 1;
    }

    printf("Ready to use socket API.\n");

    Socket::deInit();

    return 0;
}
