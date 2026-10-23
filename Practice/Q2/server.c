
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    int s, c, n, length;
    char buffer[1024] = {0};
    char reply[1024];
    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);

    s = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = INADDR_ANY;

    bind(s, (struct sockaddr *)&server, sizeof(server));
    listen(s, 5);

    printf("TCP server waiting...\n");

    c = accept(s, (struct sockaddr *)&client, &len);

    while (1)
    {
        n = recv(c, buffer, sizeof(buffer) - 1, 0);

        if (n <= 0)
            break;

        buffer[n] = '\0';
        printf("Client: %s\n", buffer);

        if (strcmp(buffer, "stop") == 0)
            break;

        length = strlen(buffer);

        if (length % 2 != 0)
        {
            for (int i = 0; i < length; i++)
                reply[i] = buffer[length - 1 - i];

            reply[length] = '\0';
        }
        else
        {
            sprintf(reply, "%d", length);
        }

        send(c, reply, strlen(reply), 0);
    }

    close(c);
    close(s);

    return 0;
}


/*Algorithm — TCP Server
1. Start.
2. Create a TCP socket using socket().
3. Initialize the server address and port number.
4. Bind the socket using bind().
5. Listen for client connections using listen().
6. Accept the client connection using accept().
7. Receive the message using recv().
8. If n <= 0, exit the loop.
9. Add '\0' to terminate the received string.
10. If the message is "stop", exit the loop.
11. Find the length of the message using strlen().
12. If the length is odd, reverse the message.
13. Otherwise, convert the length into a string using sprintf().
14. Send the result to the client using send().
15. Repeat steps 7–14.
16. Close the sockets.
17. Stop.*/