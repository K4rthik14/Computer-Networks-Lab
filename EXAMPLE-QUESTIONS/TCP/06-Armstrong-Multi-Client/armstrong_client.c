
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define SIZE 100

int main()
{
    int s, role, num;
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
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("connect");
        close(s);
        exit(1);
    }

    printf("Select role:\n");
    printf("1. Client 1 (Send number)\n");
    printf("2. Client 2 (Receive result)\n");
    printf("Enter choice: ");

    if (scanf("%d", &role) != 1 || (role != 1 && role != 2)) {
        printf("Invalid role\n");
        close(s);
        return 1;
    }

    if (role == 1)
    {
        printf("Enter number: ");

        if (scanf("%d", &num) != 1) {
            printf("Invalid number\n");
            close(s);
            return 1;
        }

        if (send(s, &num, sizeof(num), 0) != sizeof(num))
            perror("send");
        else {
            if (recv(s, result, SIZE - 1, 0) > 0)
                printf("Server: %s\n", result);
            else
                printf("No result sent to Client 1.\n");
        }
    }
    else
    {
        int n = recv(s, result, SIZE - 1, 0);

        if (n <= 0)
            printf("No result received.\n");
        else {
            result[n] = '\0';
            printf("Server result: %s\n", result);
        }
    }

    close(s);
    return 0;
}
