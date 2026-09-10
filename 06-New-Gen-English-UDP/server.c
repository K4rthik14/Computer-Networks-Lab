#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8008
#define BUFFER_SIZE 1024
#define OUTPUT_SIZE 2048

int main() {

    int sock;
    int n;

    char buffer[BUFFER_SIZE];
    char output[OUTPUT_SIZE];

    char *word;

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
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    // Bind socket
    if (bind(sock,
             (struct sockaddr *)&server,
             sizeof(server)) < 0) {

        perror("Bind failed");
        close(sock);
        exit(1);
    }

    printf("Server started...\n");
    printf("Waiting for sentences...\n");

    while (1) {

        // Receive sentence from client
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

        // Clear output
        output[0] = '\0';

        // Split sentence into words
        word = strtok(buffer, " ");

        while (word != NULL) {

            char punctuation[10] = "";

            int last = strlen(word) - 1;

            // Preserve punctuation
            if (last >= 0 &&
                (word[last] == ',' || word[last] == '.')) {

                punctuation[0] = word[last];
                word[last] = '\0';
            }

            // Translate abbreviations
            if (strcmp(word, "tbh") == 0) {
                strcat(output, "to be honest");
            }
            else if (strcmp(word, "ig") == 0) {
                strcat(output, "I guess");
            }
            else if (strcmp(word, "tbf") == 0) {
                strcat(output, "to be fair");
            }
            else if (strcmp(word, "atm") == 0) {
                strcat(output, "at the moment");
            }
            else if (strcmp(word, "irl") == 0) {
                strcat(output, "in real life");
            }
            else if (strcmp(word, "lol") == 0) {
                strcat(output, "laughing out loud");
            }
            else if (strcmp(word, "asap") == 0) {
                strcat(output, "as soon as possible");
            }
            else if (strcmp(word, "omg") == 0) {
                strcat(output, "oh my god");
            }
            else if (strcmp(word, "ttyl") == 0) {
                strcat(output, "talk to you later");
            }
            else if (strcmp(word, "idk") == 0) {
                strcat(output, "I don't know");
            }
            else if (strcmp(word, "nvm") == 0) {
                strcat(output, "never mind");
            }
            else if (strcmp(word, "idc") == 0) {
                strcat(output, "I don't care");
            }
            else {
                // Keep normal words unchanged
                strcat(output, word);
            }

            // Add punctuation
            strcat(output, punctuation);

            // Add space between words
            strcat(output, " ");

            word = strtok(NULL, " ");
        }

        printf("Received: %s\n", buffer);
        printf("Translated: %s\n", output);

        // Send translated sentence back
        sendto(
            sock,
            output,
            strlen(output),
            0,
            (struct sockaddr *)&client,
            len
        );
    }

    close(sock);

    return 0;
}