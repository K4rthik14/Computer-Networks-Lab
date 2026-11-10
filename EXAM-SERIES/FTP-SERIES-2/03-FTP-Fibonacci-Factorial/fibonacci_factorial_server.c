#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    int sockfd, connfd, bytes_received;
    char buffer[1024];
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(sockfd, 5);

    printf("Server waiting...\n");

    connfd = accept(sockfd, (struct sockaddr *)&client_addr, &addr_len);

    while (1)
    {
        bytes_received = recv(connfd, buffer, sizeof(buffer) - 1, 0);

        if (bytes_received <= 0)
            break;

        buffer[bytes_received] = '\0';

        if (strcmp(buffer, "exit") == 0)
            break;

        printf("Client message: %s\n", buffer);

        strcpy(buffer, "HELLO");

        send(connfd, buffer, strlen(buffer), 0);
    }

    close(connfd);
    close(sockfd);

    return 0;
}