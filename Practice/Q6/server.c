
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

int main()
{
    int s, c, n, num, temp, digit, sum, count;
    int client1 = -1, client2 = -1;
    char buffer[100];
    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);

    s = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = INADDR_ANY;

    bind(s, (struct sockaddr *)&server, sizeof(server));
    listen(s, 5);

    printf("Server waiting...\n");

    while (1)
    {
        c = accept(s, (struct sockaddr *)&client, &len);

        if (c < 0)
            continue;

        if (client1 == -1)
        {
            client1 = c;
            printf("Client 1 connected\n");
            continue;
        }
        else if (client2 == -1)
        {
            client2 = c;
            printf("Client 2 connected\n");
        }

        n = recv(client1, buffer, sizeof(buffer) - 1, 0);

        if (n <= 0)
            break;

        buffer[n] = '\0';
        num = atoi(buffer);

        temp = num;
        sum = 0;
        count = 0;

        int t = num;

        do {
            count++;
            t /= 10;
        } while (t != 0);

        t = num;

        do {
            digit = t % 10;

            int power = 1;
            for (int i = 0; i < count; i++)
                power *= digit;

            sum += power;
            t /= 10;
        } while (t != 0);

        if (sum == num)
        {
            strcpy(buffer, "Armstrong");
            send(client2, buffer, strlen(buffer), 0);
        }
        else
        {
            strcpy(buffer, "Not Armstrong");
            send(client1, buffer, strlen(buffer), 0);
        }

        close(client1);
        close(client2);
        client1 = -1;
        client2 = -1;
    }

    close(s);
    return 0;
}

/*Algorithm — TCP Server
1. Start.
2. Create a TCP socket.
3. Bind the socket to the server port.
4. Listen for incoming client connections.
5. Accept Client 1 and Client 2.
6. Receive the integer from Client 1.
7. Count the number of digits in the integer.
8. Calculate the sum of each digit raised to the power of the digit count.
9. Compare the sum with the original number.
10. If equal, send "Armstrong" to Client 2.
11. Otherwise, send "Not Armstrong" to Client 1.
12. Close both client sockets.
13. Repeat if required.
14. Close the server socket and stop.*/