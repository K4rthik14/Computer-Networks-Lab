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

    printf("Enter filename: ");
    scanf("%99s", filename);

    while (1)
    {
        file_ptr = fopen(filename, "r");

        if (file_ptr == NULL)
        {
            printf("File not found!\n");
            break;
        }

        fgets(buffer, sizeof(buffer), file_ptr);
        fclose(file_ptr);

        if (strcmp(buffer, "exit") == 0)
        {
            send(sockfd, "exit", 4, 0);
            break;
        }

        send(sockfd, buffer, strlen(buffer), 0);

        bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0);

        if (bytes_received <= 0)
            break;

        buffer[bytes_received] = '\0';

        int message_length = strlen(buffer);

        file_ptr = fopen(filename, "w");

        if (message_length % 2 != 0)
        {
            int first = 0, second = 1;

            fprintf(file_ptr, "Fibonacci: ");

            for (int i = 0; i < message_length; i++)
            {
                fprintf(file_ptr, "%d ", first);

                int next = first + second;
                first = second;
                second = next;
            }
        }
        else
        {
            unsigned long long factorial = 1;

            for (int i = 1; i <= message_length; i++)
                factorial *= i;

            fprintf(file_ptr, "Factorial: %llu", factorial);
        }

        fclose(file_ptr);

        printf("Result written to %s\n", filename);
    }

    close(sockfd);

    return 0;
}