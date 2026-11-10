
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
    int s, i, j, k, temp, n;
    struct data d;
    struct result res;

    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);

    // 1. Create UDP socket
    s = socket(AF_INET, SOCK_DGRAM, 0);

    // 2. Configure server address
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = INADDR_ANY;

    // 3. Bind
    bind(s, (struct sockaddr *)&server, sizeof(server));

    printf("Server waiting...\n");

    // 4. Receive matrices
    n = recvfrom(s, &d, sizeof(d), 0,
                 (struct sockaddr *)&client, &len);

    if(n != sizeof(d))
    {
        printf("Invalid data received\n");
        close(s);
        return 1;
    }

    printf("Matrices received\n");

    // 5. Check dimensions
    if(d.c1 != d.r2)
    {
        printf("Matrix multiplication not possible\n");
        close(s);
        return 1;
    }

    res.rows = d.r1;
    res.cols = d.c2;

    // 6. Initialize resultant matrix
    for(i = 0; i < res.rows; i++)
    {
        for(j = 0; j < res.cols; j++)
        {
            res.c[i][j] = 0;
        }
    }

    // 7. Matrix multiplication
    for(i = 0; i < d.r1; i++)
    {
        for(j = 0; j < d.c2; j++)
        {
            for(k = 0; k < d.c1; k++)
            {
                res.c[i][j] += d.a[i][k] * d.b[k][j];
            }
        }
    }

    printf("Resultant matrix:\n");

    for(i = 0; i < res.rows; i++)
    {
        for(j = 0; j < res.cols; j++)
        {
            printf("%d ", res.c[i][j]);
        }
        printf("\n");
    }

    // 8. Sort resultant matrix elements
    int total = res.rows * res.cols;

    for(i = 0; i < total-1; i++)
    {
        for(j = 0; j < total-i-1; j++)
        {
            if(res.c[j/res.cols][j%res.cols] >
               res.c[(j+1)/res.cols][(j+1)%res.cols])
            {
                temp = res.c[j/res.cols][j%res.cols];

                res.c[j/res.cols][j%res.cols] =
                res.c[(j+1)/res.cols][(j+1)%res.cols];

                res.c[(j+1)/res.cols][(j+1)%res.cols] = temp;
            }
        }
    }

    // 9. Display sorted matrix
    printf("Sorted resultant matrix:\n");

    for(i = 0; i < res.rows; i++)
    {
        for(j = 0; j < res.cols; j++)
        {
            printf("%d ", res.c[i][j]);
        }
        printf("\n");
    }

    // 10. Send sorted matrix to client
    sendto(s, &res, sizeof(res), 0,
           (struct sockaddr *)&client, len);

    close(s);

    return 0;
}
