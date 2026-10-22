
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main()
{
    int s;
    char buffer[1024] = {0};
    struct sockaddr_in server;
    socklen_t len = sizeof(server);

    // 1. Create UDP socket
    s = socket(AF_INET, SOCK_DGRAM, 0);

    // 2. Prepare server address
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

    // 3. Send datagram
    sendto(s, "Hello from client", 17, 0,
           (struct sockaddr *)&server, len);

    // 4. Receive reply
    int n = recvfrom(s, buffer, sizeof(buffer) - 1, 0,
                     NULL, NULL);

    if (n >= 0) {
        buffer[n] = '\0';
        printf("Server: %s\n", buffer);
    }

    // 5. Close socket
    close(s);

    return 0;
}
