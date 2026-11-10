
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main()
{
    int s, c, n, count = 0;
    char buffer[200], reply[20];

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

    printf("Server waiting...\n");

    // 5. Accept client
    c = accept(s, (struct sockaddr *)&client, &len);

    // 6. Receive message
    n = recv(c, buffer, sizeof(buffer)-1, 0);

    if(n <= 0)
    {
        close(c);
        close(s);
        return 1;
    }

    buffer[n] = '\0';

    printf("Message received: %s\n", buffer);

    // 7. Find length without strlen()
    for(int i = 0; buffer[i] != '\0'; i++)
    {
        count++;
    }

    printf("Length: %d\n", count);

    // 8. Check odd or even
    if(count % 2 == 0)
    {
        strcpy(reply, "even");
    }
    else
    {
        strcpy(reply, "odd");
    }

    // 9. Send result to client
    send(c, reply, strlen(reply), 0);

    printf("Sent: %s\n", reply);

    close(c);
    close(s);

    return 0;
}
