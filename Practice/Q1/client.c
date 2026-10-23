
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    int s, n;
    char buffer[1024];
    struct sockaddr_in server;
    socklen_t len = sizeof(server);

    s = socket(AF_INET, SOCK_DGRAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("Enter message: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';

    sendto(s, buffer, strlen(buffer), 0,
           (struct sockaddr *)&server, len);

    n = recvfrom(s, buffer, sizeof(buffer) - 1, 0,
                 NULL, NULL);

    if (n < 0) {
        perror("recvfrom");
        close(s);
        return 1;
    }

    buffer[n] = '\0';
    printf("Server reply: %s\n", buffer);

    close(s);

    return 0;
}


/*
Start.

Create a UDP socket using socket().

Initialize the server address and port.

Read a message from the user.

Send the message using sendto().

Receive the reply using recvfrom().

Add '\0' to terminate the received string.

Display the server's reply.

Close the socket.

Stop.
*/