
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main()
{
    int s;
    char buffer[1024] = {0};
    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);

    // 1. Create UDP socket
    s = socket(AF_INET, SOCK_DGRAM, 0);

    // 2. Prepare server address
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = INADDR_ANY;

    // 3. Bind socket
    bind(s, (struct sockaddr *)&server, sizeof(server));

    printf("UDP server waiting...\n");

    // 4. Receive datagram
    int n = recvfrom(s, buffer, sizeof(buffer) - 1, 0,
                     (struct sockaddr *)&client, &len);

    if (n >= 0) {
        buffer[n] = '\0';
        printf("Client: %s\n", buffer);

        // 5. Send reply
        sendto(s, "Hello from server", 17, 0,
               (struct sockaddr *)&client, len);
    }

    // 6. Close socket
    close(s);

    return 0;
}
