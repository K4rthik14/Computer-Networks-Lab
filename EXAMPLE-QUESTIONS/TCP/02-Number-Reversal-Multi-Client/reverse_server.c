
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

// Function to reverse a string
void reverse(char str[])
{
    int l = strlen(str);

    for(int i = 0; i < l/2; i++)
    {
        char temp = str[i];
        str[i] = str[l-i-1];
        str[l-i-1] = temp;
    }
}

int main()
{
    int s, c1, c2, n;
    char num1[100], num2[100];

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

    printf("Server waiting for clients...\n");

    // 5. Accept Client 1
    c1 = accept(s, (struct sockaddr *)&client, &len);
    printf("Client 1 connected\n");

    // 6. Accept Client 2
    len = sizeof(client);
    c2 = accept(s, (struct sockaddr *)&client, &len);
    printf("Client 2 connected\n");

    // 7. Receive number from Client 1
    n = recv(c1, num1, sizeof(num1)-1, 0);

    if(n <= 0)
    {
        close(c1);
        close(c2);
        close(s);
        return 1;
    }

    num1[n] = '\0';

    // 8. Receive number from Client 2
    n = recv(c2, num2, sizeof(num2)-1, 0);

    if(n <= 0)
    {
        close(c1);
        close(c2);
        close(s);
        return 1;
    }

    num2[n] = '\0';

    printf("Client 1 number: %s\n", num1);
    printf("Client 2 number: %s\n", num2);

    // 9. Reverse both numbers
    reverse(num1);
    reverse(num2);

    printf("Reversed Client 1 number: %s\n", num1);
    printf("Reversed Client 2 number: %s\n", num2);

    // 10. Send Client 1's reversed number to Client 2
    send(c2, num1, strlen(num1), 0);

    // 11. Send Client 2's reversed number to Client 1
    send(c1, num2, strlen(num2), 0);

    // 12. Close connections
    close(c1);
    close(c2);
    close(s);

    return 0;
}
