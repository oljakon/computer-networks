#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <time.h>

#define MSG_LEN 500
#define SOCK_ADDR "localhost"
#define SOCK_PORT 5000

#define BUFLEN 500


std::string generate_get(std::string filename)
{
    const std::string version = "HTTP/1.1";
    return "GET /" + filename + " " + version + "\r\n" + "User: console-client-pid-" + std::to_string(getpid());
}

int main(void)
{
    srand(time(NULL));
    char message[MSG_LEN];

    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0)
    {
        perror("Error in sock()");
        return sock;
    }

    struct hostent* host = gethostbyname(SOCK_ADDR);
    if (!host)
    {
        perror("Error in gethostbyname()");
        return -1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SOCK_PORT);
    server_addr.sin_addr = *((struct in_addr*) host->h_addr_list[0]);

    if (connect(sock, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0)
    {
        perror("Error in connect()");
        return -1;
    }

    printf("Enter an URL: \n");
    scanf("%s", message);
    std::string request = generate_get(message);
    if (send(sock, request.c_str(), request.length(), 0) < 0)
    {
        perror("Error in send()");
        return -1;
    }
    printf("Client has send a get request, waiting for response\n");

    char buf[BUFLEN];
    if (recv(sock, buf, BUFLEN, 0) < 0)
    {
        perror("Error in recv()");
        return -1;
    }

    printf("Client recieved an answer:\n\n%s", buf);

    close(sock);
    return 0;
}
