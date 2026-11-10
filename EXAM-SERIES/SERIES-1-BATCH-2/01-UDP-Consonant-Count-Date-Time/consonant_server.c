
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define SIZE 1024

// Function to count consonants
int count_consonants(char str[])
{
    int count = 0;

    for (int i = 0; str[i] != '\0'; i++)
    {
        char ch = tolower((unsigned char)str[i]);

        if (isalpha((unsigned char)ch) &&
            ch != 'a' && ch != 'e' &&
            ch != 'i' && ch != 'o' &&
            ch != 'u')
        {
            count++;
        }
    }

    return count;
}

// Function to check prime number
int is_prime(int n)
{
    if (n < 2)
        return 0;

    for (int i = 2; i <= n / i; i++)
    {
        if (n % i == 0)
            return 0;
    }

    return 1;
}

int main()
{
    int s, n;
    char buffer[SIZE];
    char response[SIZE];

    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);

    // 1. Create UDP socket
    s = socket(AF_INET, SOCK_DGRAM, 0);

    if (s < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    printf("UDP socket created successfully\n");

    // 2. Configure server address
    memset(&server, 0, sizeof(server));

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    // 3. Bind socket
    if (bind(s, (struct sockaddr *)&server,
             sizeof(server)) < 0)
    {
        perror("Bind failed");
        close(s);
        exit(EXIT_FAILURE);
    }

    printf("Server is waiting for client...\n");

    // 4. Receive client message
    n = recvfrom(s, buffer, SIZE - 1, 0,
                 (struct sockaddr *)&client, &len);

    if (n < 0)
    {
        perror("Receive failed");
        close(s);
        exit(EXIT_FAILURE);
    }

    buffer[n] = '\0';

    printf("Client message: %s\n", buffer);

    // 5. Count consonants
    int count = count_consonants(buffer);

    printf("Number of consonants: %d\n", count);

    // 6. Check whether count is prime
    if (is_prime(count))
    {
        time_t now;
        struct tm current_time;

        char datetime[100];

        // Get current date and time
        time(&now);

        if (localtime_r(&now, &current_time) == NULL)
        {
            perror("Time conversion failed");
            close(s);
            exit(EXIT_FAILURE);
        }

        strftime(datetime, sizeof(datetime),
                 "%d-%m-%Y %H:%M:%S",
                 &current_time);

        snprintf(response, sizeof(response),
                 "Prime count! Current date and time: %s",
                 datetime);
    }
    else
    {
        strcpy(response, "sorry");
    }

    // 7. Send response to client
    int sent = sendto(s, response, strlen(response), 0,
                      (struct sockaddr *)&client, len);

    if (sent < 0)
    {
        perror("Send failed");
        close(s);
        exit(EXIT_FAILURE);
    }

    printf("Response sent: %s\n", response);

    // 8. Close socket
    close(s);

    return 0;
}
