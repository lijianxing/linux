#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "us_xfr.h"

#define BACKLOG 128

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

    if (remove(SV_SOCK_PATH) == -1 && errno != ENOENT) {
        printf("error remove %s\n", SV_SOCK_PATH);
        return -1;
    }

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path)-1);
    if (bind(sfd, (struct sockaddr*)&addr, sizeof(struct sockaddr_un)) == -1) {
        printf("error bind\n");
        return -1;
    }
    if (listen(sfd, BACKLOG) == -1) {
        printf("error listen\n");
        return -1;
    }

    for (;;) {
        cfd = accept(sfd, NULL, NULL);
        if (cfd == -1) {
            printf("error accept");
            return -1;
        }
        while ((numRead = read(cfd, buf, BUF_SIZE)) > 0) {
            if (write(STDOUT_FILENO, buf, numRead) != numRead) {
                printf("error write");
                return -1;
            }
        }

        if (numRead == -1) {
            printf("error read");
            return -1;
        }

        if (close(cfd) == -1) {
            printf("error close");
        }
    }
    return 0;
}


