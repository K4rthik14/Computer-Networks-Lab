
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main()
{
    int s, c;
    char buffer[1024] = {0};
    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);

    // 1. Create TCP socket
    s = socket(AF_INET, SOCK_STREAM, 0);

    // 2. Prepare server address
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = INADDR_ANY;

    // 3. Bind socket
    bind(s, (struct sockaddr *)&server, sizeof(server));

    // 4. Listen for connections
    listen(s, 5);
    printf("TCP server waiting...\n");

    // 5. Accept client connection
    c = accept(s, (struct sockaddr *)&client, &len);

    // 6. Receive message
    int n = recv(c, buffer, sizeof(buffer) - 1, 0);

    if (n > 0) {
        buffer[n] = '\0';
        printf("Client: %s\n", buffer);

        // 7. Send reply
        send(c, "Hello from server", 17, 0);
    }

    // 8. Close sockets
    close(c);
    close(s);

    return 0;
}
