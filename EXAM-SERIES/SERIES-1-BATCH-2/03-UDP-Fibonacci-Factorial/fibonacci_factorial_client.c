
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define SIZE 1024

int main()
{
    int s, n;
    char buffer[SIZE];
    struct sockaddr_in server;
    socklen_t len = sizeof(server);

    s = socket(AF_INET, SOCK_DGRAM, 0);

    if (s < 0) {
        perror("socket");
        exit(1);
    }

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    while (1)
    {
        printf("Enter message: ");

        if (fgets(buffer, SIZE, stdin) == NULL)
            break;

        buffer[strcspn(buffer, "\n")] = '\0';

        if (sendto(s, buffer, strlen(buffer), 0,
                   (struct sockaddr *)&server, len) < 0) {
            perror("sendto");
            break;
        }

        if (strcmp(buffer, "exit") == 0)
            break;

        memset(buffer, 0, SIZE);

        n = recvfrom(s, buffer, SIZE - 1, 0, NULL, NULL);

        if (n < 0) {
            perror("recvfrom");
            break;
        }

        buffer[n] = '\0';

        int l = strlen(buffer);

        printf("Server replied: %s\n", buffer);

        if (l % 2 != 0)
        {
            int a = 0, b = 1;

            printf("Fibonacci series: ");

            for (int i = 0; i < l; i++)
            {
                printf("%d ", a);

                int c = a + b;
                a = b;
                b = c;
            }

            printf("\n");
        }
        else
        {
            unsigned long long fact = 1;

            if (l > 20) {
                printf("Factorial overflow!\n");
                continue;
            }

            for (int i = 1; i <= l; i++)
                fact *= i;

            printf("Factorial of %d = %llu\n", l, fact);
        }
    }

    close(s);
    return 0;
}
