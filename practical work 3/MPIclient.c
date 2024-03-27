#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <fcntl.h>

#define MAX 1024
#define PORT 8080

void receive_file(char *filename) {
    char buff[MAX];
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd < 0) {
        printf("Error creating/opening file.\n");
        return;
    }

    MPI_Status status;
    while (1) {
        MPI_Recv(buff, MAX, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
        int count;
        MPI_Get_count(&status, MPI_CHAR, &count);
        if (count <= 0) break;
        write(fd, buff, count);
    }
    close(fd);
}

int main(int argc, char **argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 1) {
        char *filename = "client_file.txt";

        printf("Connected to the server..\n");

        MPI_Send(filename, strlen(filename) + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        receive_file(filename);
    }

    MPI_Finalize();
    return 0;
}
