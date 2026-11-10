#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    int sockfd, connfd, bytes_received;
    int numbers[100], count, temp;
    int largest, second_largest;
    char buffer[4096], result[4096];

    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    FILE *file_ptr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(sockfd, 5);

    printf("Server waiting...\n");

    connfd = accept(sockfd, (struct sockaddr *)&client_addr, &addr_len);

    // Receive count and numbers
    bytes_received = recv(connfd, buffer, sizeof(buffer) - 1, 0);

    if (bytes_received <= 0)
        return 1;

    buffer[bytes_received] = '\0';

    char *token = strtok(buffer, " \n");
    count = atoi(token);

    for (int i = 0; i < count; i++)
    {
        token = strtok(NULL, " \n");
        numbers[i] = atoi(token);
    }

    // Sort in ascending order
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = i + 1; j < count; j++)
        {
            if (numbers[i] > numbers[j])
            {
                temp = numbers[i];
                numbers[i] = numbers[j];
                numbers[j] = temp;
            }
        }
    }

    // Write sorted numbers
    file_ptr = fopen("sort.txt", "w");

    for (int i = 0; i < count; i++)
        fprintf(file_ptr, "%d ", numbers[i]);

    fclose(file_ptr);

    // Find second-largest distinct number
    largest = numbers[count - 1];
    second_largest = largest;

    int found = 0;

    for (int i = count - 2; i >= 0; i--)
    {
        if (numbers[i] < largest)
        {
            second_largest = numbers[i];
            found = 1;
            break;
        }
    }

    // Write second-largest number
    file_ptr = fopen("second.txt", "w");

    if (found)
        fprintf(file_ptr, "%d", second_largest);
    else
        fprintf(file_ptr, "No second-largest distinct number");

    fclose(file_ptr);

    // Send sorted file content
    file_ptr = fopen("sort.txt", "r");
    int file_size = fread(result, 1, sizeof(result), file_ptr);
    fclose(file_ptr);

    send(connfd, result, file_size, 0);

    // Send second-largest file content
    file_ptr = fopen("second.txt", "r");
    file_size = fread(result, 1, sizeof(result), file_ptr);
    fclose(file_ptr);

    send(connfd, result, file_size, 0);

    close(connfd);
    close(sockfd);

    return 0;
}