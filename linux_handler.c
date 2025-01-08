#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>

#define PORT 1234
#define CLIENT_IP "0.0.0.0"
#define READSIZE 1024

void die(const char* msg)
{
    fprintf(stderr, "[%d] %s\n", errno, msg);
    abort();
}

void client_com(int fd)
{
    char buffer[READSIZE];
    size_t byte_read = read(fd, buffer, sizeof(buffer) - 1);
    while (true)
    {
        if (byte_read < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }
            break;
        }
        if (byte_read == 0)
        {
            printf("remote shell disconnected");
            break;
        }
    }
}


int main()
{
    int fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0 );
    if (fd < 0)
    {
        die("Socket() Failed");
    }
    int opt = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        die("setsocketopt() Failed");
    }
    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(PORT);
    addr.sin_addr.s_addr = inet_addr(CLIENT_IP);
    if (bind(fd, (const struct sockaddr*)& addr, sizeof(addr)))
    {
        die("bind() failed");
    }
    if (listen(fd, SOMAXCONN) != 0)
    {
        die("listen() failed");
    }
    while (true)
    {
        struct sockaddr_in client_addr = {};
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(fd, (struct sockaddr*)& client_addr, &client_len);
        if (client_fd < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                sleep(1);
                continue;
            }
            break;
        }
        printf("New connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        client_com(fd);
    }
    close(fd);
    return EXIT_SUCCESS;
}
