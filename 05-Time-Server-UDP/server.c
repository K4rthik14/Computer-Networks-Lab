#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8008
#define BUFFER_SIZE 1024

int main() {

    int sock;
    int n;

    char buffer[BUFFER_SIZE];

    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);

    // Create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (sock < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Configure server address
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // Bind socket
    if (bind(sock,
             (struct sockaddr *)&server,
             sizeof(server)) < 0) {

        perror("Bind failed");
        close(sock);
        exit(1);
    }

    printf("Time Server started...\n");
    printf("Waiting for requests...\n");

    while (1) {

        // Receive request from client
        n = recvfrom(
            sock,
            buffer,
            BUFFER_SIZE - 1,
            0,
            (struct sockaddr *)&client,
            &len
        );

        if (n < 0) {
            perror("recvfrom failed");
            continue;
        }

        buffer[n] = '\0';

        printf("Time request received.\n");

        // Create child process for concurrent handling
        if (fork() == 0) {

            char time_string[BUFFER_SIZE];

            // Get current system time
            time_t current_time;
            time(&current_time);

            strcpy(time_string, ctime(&current_time));

            // Send time to client
            sendto(
                sock,
                time_string,
                strlen(time_string),
                0,
                (struct sockaddr *)&client,
                len
            );

            printf("Time sent to client.\n");

            exit(0);
        }
    }

    close(sock);

    return 0;
}