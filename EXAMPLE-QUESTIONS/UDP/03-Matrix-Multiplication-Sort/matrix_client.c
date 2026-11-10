
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define MAX 10

struct data
{
    int r1, c1, r2, c2;
    int a[MAX][MAX];
    int b[MAX][MAX];
};

struct result
{
    int rows, cols;
    int c[MAX][MAX];
};

int main()
{
    int s, i, j, n;
    struct data d;
    struct result res;

    struct sockaddr_in server;

    // 1. Create UDP socket
    s = socket(AF_INET, SOCK_DGRAM, 0);

    // 2. Configure server address
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    // 3. Read first matrix dimensions
    printf("Enter rows and columns of first matrix: ");
    scanf("%d%d", &d.r1, &d.c1);

    // 4. Read second matrix dimensions
    printf("Enter rows and columns of second matrix: ");
    scanf("%d%d", &d.r2, &d.c2);

    // 5. Check dimensions
    if(d.r1 <= 0 || d.c1 <= 0 ||
       d.r2 <= 0 || d.c2 <= 0 ||
       d.r1 > MAX || d.c1 > MAX ||
       d.r2 > MAX || d.c2 > MAX ||
       d.c1 != d.r2)
    {
        printf("Invalid matrix dimensions\n");
        close(s);
        return 1;
    }

    // 6. Read first matrix
    printf("Enter first matrix elements:\n");

    for(i = 0; i < d.r1; i++)
    {
        for(j = 0; j < d.c1; j++)
        {
            scanf("%d", &d.a[i][j]);
        }
    }

    // 7. Read second matrix
    printf("Enter second matrix elements:\n");

    for(i = 0; i < d.r2; i++)
    {
        for(j = 0; j < d.c2; j++)
        {
            scanf("%d", &d.b[i][j]);
        }
    }

    // 8. Send both matrices
    sendto(s, &d, sizeof(d), 0,
           (struct sockaddr *)&server, sizeof(server));

    printf("Matrices sent to server\n");

    // 9. Receive sorted resultant matrix
    n = recvfrom(s, &res, sizeof(res), 0, NULL, NULL);

    if(n != sizeof(res))
    {
        printf("Invalid response received\n");
        close(s);
        return 1;
    }

    // 10. Display sorted matrix
    printf("Sorted resultant matrix:\n");

    for(i = 0; i < res.rows; i++)
    {
        for(j = 0; j < res.cols; j++)
        {
            printf("%d ", res.c[i][j]);
        }
        printf("\n");
    }

    close(s);

    return 0;
}
