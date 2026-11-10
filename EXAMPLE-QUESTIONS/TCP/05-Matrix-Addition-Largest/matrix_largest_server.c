
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define MAX 10

// Receive all requested bytes
int receive_all(int fd, void *data, size_t size)
{
    size_t total = 0;
    char *p = data;

    while (total < size)
    {
        ssize_t n = recv(fd, p + total, size - total, 0);

        if (n <= 0)
            return -1;

        total += n;
    }

    return 0;
}

// Send all requested bytes
int send_all(int fd, const void *data, size_t size)
{
    size_t total = 0;
    const char *p = data;

    while (total < size)
    {
        ssize_t n = send(fd, p + total, size - total, 0);

        if (n <= 0)
            return -1;

        total += n;
    }

    return 0;
}

int main()
{
    int s, c, n;
    int A[MAX][MAX], B[MAX][MAX], C[MAX][MAX];
    int largest;

    struct sockaddr_in server;

    s = socket(AF_INET, SOCK_STREAM, 0);

    if (s < 0) {
        perror("socket");
        exit(1);
    }

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    int opt = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(s, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("bind");
        close(s);
        exit(1);
    }

    if (listen(s, 5) < 0) {
        perror("listen");
        close(s);
        exit(1);
    }

    printf("Server waiting...\n");

    c = accept(s, NULL, NULL);

    if (c < 0) {
        perror("accept");
        close(s);
        exit(1);
    }

    if (receive_all(c, &n, sizeof(n)) < 0 || n < 1 || n > MAX) {
        printf("Invalid matrix size or receive error\n");
        close(c);
        close(s);
        return 1;
    }

    if (receive_all(c, A, n * n * sizeof(int)) < 0 ||
        receive_all(c, B, n * n * sizeof(int)) < 0) {
        perror("Matrix receive failed");
        close(c);
        close(s);
        return 1;
    }

    largest = -2147483647 - 1;

    printf("Resultant matrix:\n");

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            C[i][j] = A[i][j] + B[i][j];

            if (C[i][j] > largest)
                largest = C[i][j];

            printf("%d ", C[i][j]);
        }

        printf("\n");
    }

    if (send_all(c, C, n * n * sizeof(int)) < 0 ||
        send_all(c, &largest, sizeof(largest)) < 0) {
        perror("send failed");
    }

    close(c);
    close(s);

    return 0;
}
