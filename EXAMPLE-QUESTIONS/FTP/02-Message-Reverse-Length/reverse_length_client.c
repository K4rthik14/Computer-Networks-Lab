#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    int sockfd, bytes_received;
    char filename[100], buffer[1024];
    struct sockaddr_in server_addr;
    FILE *file_ptr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr *)&server_addr,
            sizeof(server_addr));

    while (1)
    {
        printf("Enter filename (or stop): ");
        scanf("%99s", filename);

        if (strcmp(filename, "stop") == 0)
        {
            send(sockfd, "stop", 4, 0);
            break;
        }

        file_ptr = fopen(filename, "r");

        if (file_ptr == NULL)
        {
            printf("File not found!\n");
            continue;
        }

        fgets(buffer, sizeof(buffer), file_ptr);
        fclose(file_ptr);

        send(sockfd, buffer, strlen(buffer), 0);

        bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0);

        if (bytes_received <= 0)
            break;

        buffer[bytes_received] = '\0';
        printf("Server: %s\n", buffer);
    }

    close(sockfd);

    return 0;
}