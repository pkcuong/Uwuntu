#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MAX_FILENAME_LENGTH 256
#define CHUNK_SIZE 1024

void send_file(const char *filename, int dest_rank) {
    char buff[CHUNK_SIZE];
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        fprintf(stderr, "Error opening file.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int bytes_read;
    while ((bytes_read = fread(buff, sizeof(char), CHUNK_SIZE, file)) > 0) {
        MPI_Send(buff, bytes_read, MPI_CHAR, dest_rank, 0, MPI_COMM_WORLD);
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    char filename[MAX_FILENAME_LENGTH] = "MPI_server_file.txt"; 

    if (world_rank == 0) {
        // master 
        int num_processes;
        MPI_Comm_size(MPI_COMM_WORLD, &num_processes);

        for (int dest_rank = 1; dest_rank < num_processes; dest_rank++) {
            send_file(filename, dest_rank);
        }
    } else {
        // slave
        receive_file(filename);
    }

    MPI_Finalize();
    return 0;
}
