#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <fcntl.h>

#define MAX 1024
#define PORT 8080

void send_file(char *filename) {
    char buff[MAX];
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        printf("Error opening file.\n");
        return;
    }

    while (1) {
        int bytes_read = read(fd, buff, MAX);
        if (bytes_read <= 0) break;
        MPI_Send(buff, bytes_read, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }
    close(fd);
}

int main(int argc, char **argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        printf("Server listening..\n");

        MPI_Status status;
        char filename[MAX];
        MPI_Recv(filename, MAX, MPI_CHAR, 1, 0, MPI_COMM_WORLD, &status);

        send_file(filename);
    }

    MPI_Finalize();
    return 0;
}
