
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
    int s, n, a[MAX], temp;
    int second;
    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);

    s = socket(AF_INET, SOCK_DGRAM, 0);

    if (s < 0) {
        perror("socket");
        exit(1);
    }

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    if (bind(s, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("bind");
        close(s);
        exit(1);
    }

    printf("UDP server waiting...\n");

    if (recvfrom(s, &n, sizeof(n), 0,
                 (struct sockaddr *)&client, &len) != sizeof(n)) {
        perror("receive count");
        close(s);
        return 1;
    }

    if (n < 2 || n > MAX) {
        second = -1;
        sendto(s, &second, sizeof(second), 0,
               (struct sockaddr *)&client, len);
        close(s);
        return 0;
    }

    if (recvfrom(s, a, n * sizeof(int), 0,
                 (struct sockaddr *)&client, &len) != n * sizeof(int)) {
        perror("receive array");
        close(s);
        return 1;
    }

    // Bubble sort
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (a[j] > a[j + 1])
            {
                temp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = temp;
            }
        }
    }

    printf("Sorted array: ");

    for (int i = 0; i < n; i++)
        printf("%d ", a[i]);

    printf("\n");

    // Find second-largest distinct element
    int largest = a[n - 1];
    second = -1;

    for (int i = n - 2; i >= 0; i--)
    {
        if (a[i] != largest) {
            second = a[i];
            break;
        }
    }

    sendto(s, &second, sizeof(second), 0,
           (struct sockaddr *)&client, len);

    close(s);
    return 0;
}
