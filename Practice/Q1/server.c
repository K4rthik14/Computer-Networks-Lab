
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

int main()
{
    int s, n, count = 0, prime = 1;
    char buffer[1024], reply[100];
    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);
    time_t now;

    s = socket(AF_INET, SOCK_DGRAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = INADDR_ANY;

    bind(s, (struct sockaddr *)&server, sizeof(server));

    n = recvfrom(s, buffer, sizeof(buffer) - 1, 0,
                 (struct sockaddr *)&client, &len);

    if (n < 0) {
        perror("recvfrom");
        close(s);
        return 1;
    }

    buffer[n] = '\0';

    for (int i = 0; buffer[i] != '\0'; i++)
    {
        char ch = buffer[i];

        if ((ch >= 'a' && ch <= 'z') ||
            (ch >= 'A' && ch <= 'Z'))
        {
            if (ch != 'a' && ch != 'e' && ch != 'i' &&
                ch != 'o' && ch != 'u' &&
                ch != 'A' && ch != 'E' && ch != 'I' &&
                ch != 'O' && ch != 'U')
            {
                count++;
            }
        }
    }

    printf("Number of consonants: %d\n", count);

    if (count < 2)
        prime = 0;
    else
    {
        for (int i = 2; i <= count / 2; i++)
        {
            if (count % i == 0)
            {
                prime = 0;
                break;
            }
        }
    }

    if (prime == 1)
    {
        now = time(NULL);
        strcpy(reply, ctime(&now));
    }
    else
    {
        strcpy(reply, "sorry");
    }

    sendto(s, reply, strlen(reply), 0,
           (struct sockaddr *)&client, len);

    close(s);

    return 0;
}

/*
Algorithm — UDP Server
1. Start.
2. Create a UDP socket using socket().
3. Initialize the server address and port.
4. Bind the socket using bind().
5. Receive the client's message using recvfrom().
6. Add '\0' to terminate the received string.
7. Initialize the consonant count to zero.
8. Traverse the message character by character.
9. If a character is an alphabet and not a vowel, increment the consonant count.
10. Check whether the count is prime.
11. If prime, obtain the current date and time using time() and ctime().
12. Otherwise, prepare the message "sorry".
13. Send the reply using sendto().
14. Close the socket.
15. Stop.
*/