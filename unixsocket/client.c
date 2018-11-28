#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "us_xfr.h"

int main(int args, char *argv[]) 
{
    struct sockaddr_un addr;
    int sfd, cfd;
    ssize_t numRead;
    char buf[BUF_SIZE];

    sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sfd == -1) {
        printf("error socket\n");
        return -1;
    }

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path)-1);

    if (connect(sfd, (struct sockaddr*)&addr, sizeof(struct sockaddr_un)) == -1) {
        printf("error connect\n");
        return -1;
    }

    while ((numRead = read(STDIN_FILENO, buf, BUF_SIZE)) > 0)
    {
        if (write(sfd, buf, numRead) != numRead)
        {
            printf("error write");
            return -1;
        }
    }

    if (numRead == -1)
    {
        printf("error read");
        return -1;
    }

    return 0;
}