
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main()
{
    int s, n, count = 0;
    char buffer[200], sub[100];
    char reply[200], countbuf[20];

    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);

    // 1. Create UDP socket
    s = socket(AF_INET, SOCK_DGRAM, 0);

    // 2. Configure server address
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = INADDR_ANY;

    // 3. Bind socket
    bind(s, (struct sockaddr *)&server, sizeof(server));

    printf("Server waiting...\n");

    // 4. Receive message from client
    n = recvfrom(s, buffer, sizeof(buffer)-1, 0,
                 (struct sockaddr *)&client, &len);

    if(n <= 0)
    {
        close(s);
        return 1;
    }

    buffer[n] = '\0';

    printf("Message received: %s\n", buffer);

    // 5. Read substring from terminal
    printf("Enter substring: ");
    fgets(sub, sizeof(sub), stdin);
    sub[strcspn(sub, "\n")] = '\0';

    // 6. Count substring occurrences
    int l = strlen(buffer);
    int sl = strlen(sub);

    if(sl > 0)
    {
        for(int i = 0; i <= l-sl; i++)
        {
            int j;

            for(j = 0; j < sl; j++)
            {
                if(buffer[i+j] != sub[j])
                    break;
            }

            if(j == sl)
                count++;
        }
    }

    printf("Occurrences: %d\n", count);

    // 7. Send count to client
    snprintf(countbuf, sizeof(countbuf), "%d", count);

    sendto(s, countbuf, strlen(countbuf), 0,
           (struct sockaddr *)&client, len);

    // 8. Prepare reply
    snprintf(reply, sizeof(reply),
             "%s occurred %d times", sub, count);

    // 9. Send reply count times
    for(int i = 0; i < count; i++)
    {
        sendto(s, reply, strlen(reply), 0,
               (struct sockaddr *)&client, len);
    }

    close(s);

    return 0;
}
