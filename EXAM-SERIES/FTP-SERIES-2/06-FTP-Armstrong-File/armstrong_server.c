#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int is_armstrong(int number)
{
    int original_number = number;
    int sum = 0;
    int digits = 0;
    int temp = number;

    while (temp > 0)
    {
        digits++;
        temp /= 10;
    }

    temp = number;

    while (temp > 0)
    {
        int digit = temp % 10;
        int power = 1;

        for (int i = 0; i < digits; i++)
            power *= digit;

        sum += power;
        temp /= 10;
    }

    return sum == original_number;
}

int main()
{
    int sockfd, connfd, bytes_received;
    int number;

    char filename[100];
    char result[100];

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

    // Receive filename
    bytes_received = recv(connfd, filename, sizeof(filename) - 1, 0);

    if (bytes_received <= 0)
        return 1;

    filename[bytes_received] = '\0';

    // Open file and read number
    file_ptr = fopen(filename, "r");

    if (file_ptr == NULL)
    {
        strcpy(result, "File not found");
    }
    else
    {
        fscanf(file_ptr, "%d", &number);
        fclose(file_ptr);

        if (is_armstrong(number))
            strcpy(result, "Armstrong");
        else
            strcpy(result, "Not Armstrong");
    }

    // Write result to file
    file_ptr = fopen("result.txt", "w");
    fprintf(file_ptr, "%s", result);
    fclose(file_ptr);

    // Send result filename
    send(connfd, "result.txt", 10, 0);

    // Send result content
    send(connfd, result, strlen(result), 0);

    close(connfd);
    close(sockfd);

    return 0;
}