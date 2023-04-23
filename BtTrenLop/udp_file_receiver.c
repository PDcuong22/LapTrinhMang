#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    int portNumber = atoi(argv[1]);

    int receiver = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(portNumber);

    bind(receiver, (struct sockaddr *)&addr, sizeof(addr));

    char buf[256];
    struct sockaddr_in sender_addr;
    int sender_addr_len = sizeof(sender_addr);

    // Reveive file name
    char name[41];
    int ret = recvfrom(receiver, name, sizeof(name), 0,
                       (struct sockaddr *)&sender_addr, &sender_addr_len);
    char fileName[50];
    sprintf(fileName, "new_%s", name);
    FILE *fptr = fopen(fileName, "w");

    // Receive file content
    while (1)
    {
        ret = recvfrom(receiver, buf, sizeof(buf), 0,
                       (struct sockaddr *)&sender_addr, &sender_addr_len);
        if (ret <= 0)
            break;
        printf("%d bytes received %s: %s\n", ret,
               inet_ntoa(sender_addr.sin_addr), buf);

        fwrite(buf, 1, ret, fptr);
    }

    fclose(fptr);
}