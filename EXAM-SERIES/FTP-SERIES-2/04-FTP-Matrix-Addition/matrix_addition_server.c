#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    int sockfd, connfd, bytes_received;
    int rows, columns;
    int matrix_a[10][10], matrix_b[10][10];
    int result_matrix[10][10];

    char filename_a[100], filename_b[100];
    char buffer[4096];

    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    FILE *file_ptr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(sockfd, 5);

    printf("Server waiting...\n");

    connfd = accept(sockfd, (struct sockaddr *)&client_addr, &addr_len);

    // Receive first filename
    bytes_received = recv(connfd, filename_a, sizeof(filename_a) - 1, 0);
    if (bytes_received <= 0)
        return 1;
    filename_a[bytes_received] = '\0';

    // Receive second filename
    bytes_received = recv(connfd, filename_b, sizeof(filename_b) - 1, 0);
    if (bytes_received <= 0)
        return 1;
    filename_b[bytes_received] = '\0';

    // Open first matrix file
    file_ptr = fopen(filename_a, "r");

    if (file_ptr == NULL)
    {
        send(connfd, "failed", 6, 0);
        return 1;
    }

    fscanf(file_ptr, "%d%d", &rows, &columns);

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < columns; j++)
            fscanf(file_ptr, "%d", &matrix_a[i][j]);

    fclose(file_ptr);

    // Open second matrix file
    file_ptr = fopen(filename_b, "r");

    if (file_ptr == NULL)
    {
        send(connfd, "failed", 6, 0);
        return 1;
    }

    int second_rows, second_columns;
    fscanf(file_ptr, "%d%d", &second_rows, &second_columns);

    if (rows != second_rows || columns != second_columns)
    {
        fclose(file_ptr);
        send(connfd, "failed", 6, 0);
        return 1;
    }

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < columns; j++)
            fscanf(file_ptr, "%d", &matrix_b[i][j]);

    fclose(file_ptr);

    // Add matrices
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < columns; j++)
            result_matrix[i][j] = matrix_a[i][j] + matrix_b[i][j];

    // Write result to matrix.txt
    file_ptr = fopen("matrix.txt", "w");

    fprintf(file_ptr, "%d %d\n", rows, columns);

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
            fprintf(file_ptr, "%d ", result_matrix[i][j]);

        fprintf(file_ptr, "\n");
    }

    fclose(file_ptr);

    // Read result file
    file_ptr = fopen("matrix.txt", "r");
    int file_size = fread(buffer, 1, sizeof(buffer), file_ptr);
    fclose(file_ptr);

    // Send result to client
    send(connfd, buffer, file_size, 0);

    close(connfd);
    close(sockfd);

    return 0;
}