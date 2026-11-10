
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main()
{
    int s, n;
    char buffer[200], reply[20];

    struct sockaddr_in server;

    // 1. Create TCP socket
    s = socket(AF_INET, SOCK_STREAM, 0);

    // 2. Configure server address
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    // 3. Connect to server
    connect(s, (struct sockaddr *)&server, sizeof(server));

    // 4. Read message
    printf("Enter message: ");
    scanf("%199s", buffer);

    // 5. Send message
    send(s, buffer, strlen(buffer), 0);

    // 6. Receive response
    n = recv(s, reply, sizeof(reply)-1, 0);

    if(n <= 0)
    {
        close(s);
        return 1;
    }

    reply[n] = '\0';

    printf("Server response: %s\n", reply);

    // 7. Replace a with b if odd
    if(strcmp(reply, "odd") == 0)
    {
        for(int i = 0; buffer[i] != '\0'; i++)
        {
            if(buffer[i] == 'a')
            {
                buffer[i] = 'b';
            }
        }
    }

    // 8. Display final message
    printf("Final message: %s\n", buffer);

    close(s);

    return 0;
}
