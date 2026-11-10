#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include <arpa/inet.h>

int is_prime(int number)
{
    if (number < 2)
        return 0;

    for (int i = 2; i * i <= number; i++)
        if (number % i == 0)
            return 0;

    return 1;
}

int main()
{
    int sockfd, connfd, bytes_received;
    char filename[100], buffer[1024];
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    FILE *file_ptr;
    int vowel_count = 0;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(sockfd, 5);

    printf("Server waiting...\n");

    connfd = accept(sockfd, (struct sockaddr *)&client_addr, &addr_len);

    bytes_received = recv(connfd, filename, sizeof(filename) - 1, 0);
    if (bytes_received <= 0)
        return 1;

    filename[bytes_received] = '\0';

    file_ptr = fopen(filename, "r");

    if (file_ptr == NULL)
    {
        send(connfd, "failed to open", 14, 0);
    }
    else
    {
        send(connfd, "ok", 2, 0);

        int character;
        while ((character = fgetc(file_ptr)) != EOF)
        {
            character = tolower(character);

            if (character == 'a' || character == 'e' ||
                character == 'i' || character == 'o' ||
                character == 'u')
                vowel_count++;
        }

        fclose(file_ptr);

        printf("Vowel count: %d\n", vowel_count);

        if (is_prime(vowel_count))
        {
            time_t current_time = time(NULL);
            struct tm *time_info = localtime(&current_time);

            file_ptr = fopen("date.txt", "w");

            fprintf(file_ptr, "%s", asctime(time_info));
            fclose(file_ptr);

            file_ptr = fopen("date.txt", "r");
            fgets(buffer, sizeof(buffer), file_ptr);
            fclose(file_ptr);

            send(connfd, buffer, strlen(buffer), 0);
        }
        else
        {
            send(connfd, "sorry", 5, 0);
        }
    }

    close(connfd);
    close(sockfd);

    return 0;
}