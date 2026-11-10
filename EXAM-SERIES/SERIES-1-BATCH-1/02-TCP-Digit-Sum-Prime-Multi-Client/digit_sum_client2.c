
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main()
{
    int s, n, num, prime = 1;
    char buffer[100], reply[100];

    struct sockaddr_in server;

    // 1. Create TCP socket
    s = socket(AF_INET, SOCK_STREAM, 0);

    // 2. Configure server address
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    // 3. Connect to server
    connect(s, (struct sockaddr *)&server, sizeof(server));

    // 4. Receive sum from server
    n = recv(s, buffer, sizeof(buffer)-1, 0);

    if(n <= 0)
    {
        close(s);
        return 1;
    }

    buffer[n] = '\0';

    num = atoi(buffer);

    printf("Sum received from server: %d\n", num);

    // 5. Check prime
    if(num <= 1)
    {
        prime = 0;
    }
    else
    {
        for(int i = 2; i*i <= num; i++)
        {
            if(num % i == 0)
            {
                prime = 0;
                break;
            }
        }
    }

    // 6. Prepare reply
    if(prime == 1)
    {
        strcpy(reply, "prime");
    }
    else
    {
        strcpy(reply, "not prime");
    }

    // 7. Send result to server
    send(s, reply, strlen(reply), 0);

    printf("Result sent to server: %s\n", reply);

    close(s);

    return 0;
}
