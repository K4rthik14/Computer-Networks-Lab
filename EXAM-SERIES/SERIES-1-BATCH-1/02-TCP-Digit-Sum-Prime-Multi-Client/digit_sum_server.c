
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main()
{
    int s, c1, c2, n;
    int num, sum = 0, digit;

    char buffer[100], sumbuf[100], reply[100];

    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);

    // 1. Create TCP socket
    s = socket(AF_INET, SOCK_STREAM, 0);

    // 2. Configure server address
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = INADDR_ANY;

    // 3. Bind
    bind(s, (struct sockaddr *)&server, sizeof(server));

    // 4. Listen
    listen(s, 5);

    printf("Server waiting for clients...\n");

    // 5. Accept Client 1
    c1 = accept(s, (struct sockaddr *)&client, &len);
    printf("Client 1 connected\n");

    // 6. Accept Client 2
    len = sizeof(client);
    c2 = accept(s, (struct sockaddr *)&client, &len);
    printf("Client 2 connected\n");

    // 7. Receive number from Client 1
    n = recv(c1, buffer, sizeof(buffer)-1, 0);

    if(n <= 0)
    {
        close(c1);
        close(c2);
        close(s);
        return 1;
    }

    buffer[n] = '\0';

    num = atoi(buffer);

    printf("Number received: %d\n", num);

    // 8. Calculate sum of digits
    while(num > 0)
    {
        digit = num % 10;
        sum = sum + digit;
        num = num / 10;
    }

    printf("Sum of digits: %d\n", sum);

    // 9. Convert sum into string
    snprintf(sumbuf, sizeof(sumbuf), "%d", sum);

    // 10. Send sum to Client 2
    send(c2, sumbuf, strlen(sumbuf), 0);

    // 11. Receive prime result from Client 2
    n = recv(c2, reply, sizeof(reply)-1, 0);

    if(n > 0)
    {
        reply[n] = '\0';
        printf("Client 2 says: %s\n", reply);
    }

    // 12. Close connections
    close(c1);
    close(c2);
    close(s);

    return 0;
}
