
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    int s, n;
    char buffer[100];
    struct sockaddr_in server;

    s = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(s, (struct sockaddr *)&server, sizeof(server));

    printf("Connected to server.\n");

    printf("Enter number (Client 1 only): ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';

    send(s, buffer, strlen(buffer), 0);

    n = recv(s, buffer, sizeof(buffer) - 1, 0);

    if (n > 0)
    {
        buffer[n] = '\0';
        printf("Server: %s\n", buffer);
    }

    close(s);
    return 0;
}

/*Algorithm — TCP Client
1. Start.
2. Create a TCP socket.
3. Initialize the server address and port.
4. Connect to the server.
5. Client 1 reads and sends an integer; Client 2 waits for the result.
6. Receive the server's response when one is expected.
7. Display the response.
8. Close the socket.
9. Stop.*/