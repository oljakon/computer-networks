#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 9999
#define BUFSIZE 10

int to_binary_system(int number)
{
    int result = 0;
    int i = 1;

    while (number != 0)
    {
        result += (number % 2) * i;
        number /= 2;
        i *= 10;
    }
    
    return result;
}

int main(int argc, char *argv[])
{
    int sockfd;
    char buffer[BUFSIZE];
    
    struct sockaddr_in server_address, client_address;
    
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0)
    {
        perror("Error in socket\n");
        return -1;
    }
    
    memset(&server_address, 0, sizeof(server_address));
    memset(&client_address, 0, sizeof(client_address));
    
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = htons(INADDR_ANY);

    if ((bind(sockfd, (struct sockaddr *)&server_address, sizeof(server_address))) < 0)
    {
        perror("Error in bind\n");
        close(sockfd);
        return -1;
    }

    printf("Server is waiting\n");
    
    socklen_t len = sizeof(client_address);
    
    int data = recvfrom(sockfd, (char *)buffer, BUFSIZE, 0, (struct sockaddr*) &client_address, &len);
    if (data < 0)
    {
        perror("Error in recvfrom");
        return -1;
    }
    
    buffer[data] = 0;
    int number = atoi(buffer);
    
    printf("decimal: %d\n", number);
    printf("binary: %d\n", to_binary_system(number));
    printf("octal: %o\n", number);
    printf("hexadecimal: %x\n", number);
    
    close(sockfd);
    return 0;
}
