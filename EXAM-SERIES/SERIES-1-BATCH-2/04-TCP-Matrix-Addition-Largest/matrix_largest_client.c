
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define MAX 10

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

int main()
{
    int s, n;
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
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("connect");
        close(s);
        exit(1);
    }

    printf("Enter matrix order: ");

    if (scanf("%d", &n) != 1 || n < 1 || n > MAX) {
        printf("Invalid matrix order\n");
        close(s);
        return 1;
    }

    printf("Enter first matrix:\n");

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            scanf("%d", &A[i][j]);

    printf("Enter second matrix:\n");

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            scanf("%d", &B[i][j]);

    if (send_all(s, &n, sizeof(n)) < 0 ||
        send_all(s, A, n * n * sizeof(int)) < 0 ||
        send_all(s, B, n * n * sizeof(int)) < 0) {
        perror("send failed");
        close(s);
        return 1;
    }

    if (receive_all(s, C, n * n * sizeof(int)) < 0 ||
        receive_all(s, &largest, sizeof(largest)) < 0) {
        perror("receive failed");
        close(s);
        return 1;
    }

    printf("\nResultant matrix:\n");

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            printf("%d ", C[i][j]);

        printf("\n");
    }

    printf("Largest element: %d\n", largest);

    close(s);

    return 0;
}
