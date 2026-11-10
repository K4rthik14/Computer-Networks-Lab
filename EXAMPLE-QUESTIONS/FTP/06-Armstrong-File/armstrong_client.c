#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    int sockfd, bytes_received;
    char filename[100], result_filename[100], result[100];

    struct sockaddr_in server_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr *)&server_addr,
            sizeof(server_addr));

    printf("Enter filename: ");
    scanf("%99s", filename);

    // Send filename
    send(sockfd, filename, strlen(filename), 0);

    // Receive result filename
    bytes_received = recv(sockfd, result_filename,
                          sizeof(result_filename) - 1, 0);

    if (bytes_received <= 0)
        return 1;

    result_filename[bytes_received] = '\0';

    // Receive result content
    bytes_received = recv(sockfd, result, sizeof(result) - 1, 0);

    if (bytes_received > 0)
    {
        result[bytes_received] = '\0';

        printf("Received file: %s\n", result_filename);
        printf("Result: %s\n", result);
    }

    close(sockfd);

    return 0;
}