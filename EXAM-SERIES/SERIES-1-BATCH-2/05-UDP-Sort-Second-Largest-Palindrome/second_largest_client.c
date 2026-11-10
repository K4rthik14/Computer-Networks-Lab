
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define MAX 100

int main()
{
    int s, n, a[MAX], second;
    int original, reversed = 0, digit;

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

    printf("Enter number of elements: ");

    if (scanf("%d", &n) != 1 || n < 2 || n > MAX) {
        printf("Invalid size\n");
        close(s);
        return 1;
    }

    printf("Enter numbers:\n");

    for (int i = 0; i < n; i++)
        scanf("%d", &a[i]);

    if (sendto(s, &n, sizeof(n), 0,
               (struct sockaddr *)&server, len) < 0 ||
        sendto(s, a, n * sizeof(int), 0,
               (struct sockaddr *)&server, len) < 0) {
        perror("sendto");
        close(s);
        return 1;
    }

    if (recvfrom(s, &second, sizeof(second), 0, NULL, NULL)
        != sizeof(second)) {
        perror("recvfrom");
        close(s);
        return 1;
    }

    if (second == -1) {
        printf("No second-largest distinct element.\n");
        close(s);
        return 0;
    }

    printf("Second largest: %d\n", second);

    // Palindrome check
    if (second < 0) {
        printf("Not a palindrome.\n");
    }
    else {
        original = second;

        while (second > 0)
        {
            digit = second % 10;
            reversed = reversed * 10 + digit;
            second /= 10;
        }

        if (original == reversed)
            printf("Palindrome number\n");
        else
            printf("Not a palindrome\n");
    }

    close(s);
    return 0;
}
