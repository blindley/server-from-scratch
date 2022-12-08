#ifndef SOCKET_202212071931
#define SOCKET_202212071931

#if defined(_WIN32)
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

#endif

struct Socket {
#if defined(_WIN32)
    typedef SOCKET _socket_t;
    bool isValid() const { return socket != INVALID_SOCKET; }
    void close() { closesocket(socket); }
    static int getLastError() { return WSAGetLastError(); }

    static bool init() {
        WSADATA d;
        return WSAStartup(MAKEWORD(2, 2), &d) == 0;
    }

    static void deInit() { WSACleanup(); }
#else
    typedef int _socket_t;
    bool isValid() const { return socket >= 0; }
    void close() { ::close(socket); }
    static int getLastError() { return errno; }
    static bool init() { return true; }
    static void deInit() {}
#endif
    _socket_t socket;

    Socket() = default;
    Socket(_socket_t the_socket)
        :socket(the_socket)
    {}
};


#endif // SOCKET_202212071931
