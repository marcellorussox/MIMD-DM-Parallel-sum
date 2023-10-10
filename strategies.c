#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

double first_strategy(int menum, int nproc, double *local_numbers, double local_sum) {

    if (menum == 0) {
        for (int i = 1; i < nproc; i++) {
            double recv_sum;
            int tag = 10 + i;
            MPI_Recv(&recv_sum, 1, MPI_DOUBLE, i, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            local_sum+= recv_sum;
        }
    } else {
        int tag = 10 + menum;
        MPI_Send(&local_sum, 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);
    }

    return local_sum;
}
