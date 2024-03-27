#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MAX_FILENAME_LENGTH 256
#define CHUNK_SIZE 1024

void receive_file(const char *filename) {
    char buff[CHUNK_SIZE];
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        fprintf(stderr, "Error creating/opening file.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    MPI_Status status;
    int bytes_received;
    while (1) {
        MPI_Recv(buff, CHUNK_SIZE, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_CHAR, &bytes_received);
        if (bytes_received == 0) break;
        fwrite(buff, sizeof(char), bytes_received, file);
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    char filename[MAX_FILENAME_LENGTH] = "MPI_client_file.txt"; 

    if (world_rank == 0) {
        //master
    } else {
        // slave 
        receive_file(filename);
    }

    MPI_Finalize();
    return 0;
}
