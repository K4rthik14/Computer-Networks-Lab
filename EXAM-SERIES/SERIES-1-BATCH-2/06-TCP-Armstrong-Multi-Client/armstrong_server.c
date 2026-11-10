
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define SIZE 100

int is_armstrong(int n)
{
    int temp = n, sum = 0, digits = 0;

    if (n < 0)
        return 0;

    int x = n;

    do {
        digits++;
        x /= 10;
    } while (x != 0);

    temp = n;

    do {
        int digit = temp % 10;
        int power = 1;

        for (int i = 0; i < digits; i++)
            power *= digit;

        sum += power;
        temp /= 10;

    } while (temp != 0);

    return sum == n;
}

int main()
{
    int s, c1, c2, num;
    char result[SIZE];

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

    printf("Waiting for Client 1...\n");

    c1 = accept(s, NULL, NULL);

    if (c1 < 0) {
        perror("accept Client 1");
        close(s);
        exit(1);
    }

    printf("Client 1 connected.\n");
    printf("Waiting for Client 2...\n");

    c2 = accept(s, NULL, NULL);

    if (c2 < 0) {
        perror("accept Client 2");
        close(c1);
        close(s);
        exit(1);
    }

    printf("Client 2 connected.\n");

    if (recv(c1, &num, sizeof(num), MSG_WAITALL) != sizeof(num)) {
        printf("Failed to receive integer\n");
    }
    else if (is_armstrong(num))
    {
        strcpy(result, "Armstrong");

        send(c2, result, strlen(result) + 1, 0);

        printf("Sent Armstrong to Client 2\n");
    }
    else
    {
        strcpy(result, "Not Armstrong");

        send(c1, result, strlen(result) + 1, 0);

        printf("Sent Not Armstrong to Client 1\n");
    }

    close(c1);
    close(c2);
    close(s);

    return 0;
}
