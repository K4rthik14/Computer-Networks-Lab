#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {

    int server_fd, client_fd;
    int n, i, j;
    int a[50][50];
    int temp;

    int upper = 1;
    int lower = 1;
    int diagonal = 1;

    char type[50];

    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);

    // Create TCP socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // Configure server address
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8080);

    // Bind socket
    bind(server_fd, (struct sockaddr *)&server, sizeof(server));

    // Listen for client
    listen(server_fd, 5);

    printf("Server waiting for connection...\n");

    // Accept client connection
    client_fd = accept(
        server_fd,
        (struct sockaddr *)&client,
        &len
    );

    printf("Client connected!\n");

    // Receive matrix order
    recv(client_fd, &temp, sizeof(temp), 0);
    n = ntohl(temp);

    // Receive matrix elements
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            recv(client_fd, &temp, sizeof(temp), 0);
            a[i][j] = ntohl(temp);
        }
    }

    // Display received matrix
    printf("\nMatrix received from client:\n");

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("%d\t", a[i][j]);
        }
        printf("\n");
    }

    // Check matrix type
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {

            // Elements below main diagonal must be zero
            // for an upper triangular matrix
            if (i > j && a[i][j] != 0) {
                upper = 0;
            }

            // Elements above main diagonal must be zero
            // for a lower triangular matrix
            if (i < j && a[i][j] != 0) {
                lower = 0;
            }

            // All non-diagonal elements must be zero
            // for a diagonal matrix
            if (i != j && a[i][j] != 0) {
                diagonal = 0;
            }
        }
    }

    // Determine matrix type
    if (diagonal) {
        strcpy(type, "Diagonal Matrix");
    }
    else if (upper) {
        strcpy(type, "Upper Triangular Matrix");
    }
    else if (lower) {
        strcpy(type, "Lower Triangular Matrix");
    }
    else {
        strcpy(type, "Neither Upper nor Lower Triangular Matrix");
    }

    printf("\nMatrix Type: %s\n", type);

    // Send result to client
    send(client_fd, type, strlen(type) + 1, 0);

    // Close sockets
    close(client_fd);
    close(server_fd);

    return 0;
}