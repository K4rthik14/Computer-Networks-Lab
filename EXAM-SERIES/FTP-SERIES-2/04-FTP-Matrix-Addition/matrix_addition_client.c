#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    int sockfd, bytes_received;
    char filename_a[100], filename_b[100];
    char buffer[4096];

    struct sockaddr_in server_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr *)&server_addr,
            sizeof(server_addr));

    printf("Enter first matrix filename: ");
    scanf("%99s", filename_a);

    printf("Enter second matrix filename: ");
    scanf("%99s", filename_b);

    // Send filenames
    send(sockfd, filename_a, strlen(filename_a), 0);
    send(sockfd, filename_b, strlen(filename_b), 0);

    // Receive result
    bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0);

    if (bytes_received > 0)
    {
        buffer[bytes_received] = '\0';

        if (strcmp(buffer, "failed") == 0)
            printf("File error or matrix dimensions do not match.\n");
        else
            printf("Resultant Matrix:\n%s\n", buffer);
    }

    close(sockfd);

    return 0;
}