#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    int sockfd, bytes_received;
    char filename[100], buffer[1024];
    struct sockaddr_in server_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr *)&server_addr,
            sizeof(server_addr));

    printf("Enter filename: ");
    scanf("%99s", filename);

    send(sockfd, filename, strlen(filename), 0);

    bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received <= 0)
        return 1;

    buffer[bytes_received] = '\0';

    printf("Server: %s\n", buffer);

    if (strcmp(buffer, "ok") == 0)
    {
        bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0);

        if (bytes_received > 0)
        {
            buffer[bytes_received] = '\0';
            printf("Date and Time: %s\n", buffer);
        }
    }

    close(sockfd);

    return 0;
}