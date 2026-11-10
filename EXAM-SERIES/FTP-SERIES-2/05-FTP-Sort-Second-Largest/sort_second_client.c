#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    int sockfd, count, bytes_received;
    int numbers[100];

    char buffer[4096];
    struct sockaddr_in server_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr *)&server_addr,
            sizeof(server_addr));

    printf("Enter number of elements: ");
    scanf("%d", &count);

    printf("Enter numbers:\n");

    for (int i = 0; i < count; i++)
        scanf("%d", &numbers[i]);

    // Prepare numbers as a message
    sprintf(buffer, "%d ", count);

    for (int i = 0; i < count; i++)
    {
        char temp[20];
        sprintf(temp, "%d ", numbers[i]);
        strcat(buffer, temp);
    }

    // Send numbers
    send(sockfd, buffer, strlen(buffer), 0);

    // Receive sorted numbers
    bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0);

    if (bytes_received <= 0)
        return 1;

    buffer[bytes_received] = '\0';
    printf("\nSorted numbers: %s\n", buffer);

    // Receive second-largest number
    bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0);

    if (bytes_received > 0)
    {
        buffer[bytes_received] = '\0';
        printf("Second-largest number: %s\n", buffer);
    }

    close(sockfd);

    return 0;
}