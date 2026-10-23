
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    int s, n;
    char buffer[1024] = {0};
    struct sockaddr_in server;

    s = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(s, (struct sockaddr *)&server, sizeof(server));

    while (1)
    {
        printf("Enter message: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        send(s, buffer, strlen(buffer), 0);

        if (strcmp(buffer, "stop") == 0)
            break;

        n = recv(s, buffer, sizeof(buffer) - 1, 0);

        if (n <= 0)
            break;

        buffer[n] = '\0';
        printf("Server: %s\n", buffer);
    }

    close(s);

    return 0;
}

/*Algorithm — TCP Client
1. Start.
2. Create a TCP socket using socket().
3. Initialize the server address and port.
4. Connect to the server using connect().
5. Read a message from the user.
6. Send the message to the server using send().
7. If the message is "stop", exit the loop.
8. Receive the server's reply using recv().
9. If n <= 0, exit the loop.
10. Add '\0' to terminate the received string.
11. Display the server's reply.
12. Repeat steps 5–11.
13. Close the socket.
14. Stop. */