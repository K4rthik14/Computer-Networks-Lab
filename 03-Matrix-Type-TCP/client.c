#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

int main() {

    int sock;
    int n, i, j;
    int a[50][50];
    int temp;
    char type[50];

    struct sockaddr_in server;

    // Create TCP socket
    sock = socket(AF_INET, SOCK_STREAM, 0);

    // Configure server address
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to server
    connect(sock, (struct sockaddr *)&server, sizeof(server));

    // Input matrix order
    printf("Enter order of matrix: ");
    scanf("%d", &n);

    if (n <= 0 || n > 50) {
        printf("Invalid matrix order.\n");
        close(sock);
        return 1;
    }

    // Generate random matrix
    srand(time(NULL));

    printf("\nGenerated Matrix:\n");

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            a[i][j] = rand() % 50 + 1;
            printf("%d\t", a[i][j]);
        }
        printf("\n");
    }

    // Send matrix order
    temp = htonl(n);
    send(sock, &temp, sizeof(temp), 0);

    // Send matrix elements
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            temp = htonl(a[i][j]);
            send(sock, &temp, sizeof(temp), 0);
        }
    }

    // Receive matrix type from server
    memset(type, 0, sizeof(type));
    recv(sock, type, sizeof(type) - 1, 0);

    printf("\nMatrix Type: %s\n", type);

    // Close socket
    close(sock);

    return 0;
}