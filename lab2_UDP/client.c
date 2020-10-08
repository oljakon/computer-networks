#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 9999
#define BUFSIZE 10
#define SRV_IP "127.0.0.1"

int main()
{
    int sockfd;
    char number[BUFSIZE];
    struct sockaddr_in server_address;

    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0)
    {
        perror("Error in socket");
        return -1;
    }

    memset(&server_address, 0, sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;
    
    if (inet_aton(SRV_IP, &server_address.sin_addr) == 0)
    {
        perror("Error in inet_aton");
        close(sockfd);
        return -1;
    }

    printf("Input number: ");
    
    if (scanf("%s", number) != 1)
    {
        perror("Error while reading number\n");
        return -1;
    }

    sendto(sockfd, (const char *)number, strlen(number), 0, (const struct sockaddr *) &server_address, sizeof(server_address));

    printf("Number sent\n");
    
    close(sockfd);
    return 0;
}
