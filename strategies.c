#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "my_math.h"

void first_strategy(int menum, int nproc, double local_sum, double *global_sum) {

    double recv_sum;

    if (menum == 0) {
        for (int i = 1; i < nproc; i++) {
            int tag = 10 + i;
            MPI_Recv(&recv_sum, 1, MPI_DOUBLE, i, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            local_sum += recv_sum;
        }
        *global_sum = local_sum;
    } else {
        int tag = 10 + menum;
        MPI_Send(&local_sum, 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);
    }
}


void second_strategy(int menum, int nproc, double local_sum, double *global_sum) {

    int i;
    double recv_sum;

    int log_nproc = (int)(log_2(nproc)); // Calcola il logaritmo in base 2 di nproc

    for (i = 0; i < log_nproc; i++) {
        int tag = 20 + i;
        if ((menum % (1 << i)) == 0) {
            if ((menum % (1 << (i + 1))) == 0) {
                int partner = menum + (1 << i);
                if (partner < nproc) {
                    
                    MPI_Recv(&recv_sum, 1, MPI_DOUBLE, partner, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    local_sum += recv_sum;
                }
            } else {
                int partner = menum - (1 << i);
                if (partner >= 0) {
                    
                    MPI_Send(&local_sum, 1, MPI_DOUBLE, partner, tag, MPI_COMM_WORLD);
                }
            }
        }
    }

    if (menum == 0) {
        *global_sum = local_sum;
    }
}



void third_strategy(int menum, int nproc, double local_sum, double *global_sum) {

    int i;
    double recv_sum;

    int log_nproc = (int)(log_2(nproc)); // Calcola il logaritmo in base 2 di nproc

    for (i = 0; i < log_nproc; i++) {
        int tag = 20 + i;
        if ((menum % (1 << i)) == 0) {
            if ((menum % (1 << (i + 1))) == 0) {
                int partner = menum + (1 << i);
                if (partner < nproc) {
                    MPI_Send(&local_sum, 1, MPI_DOUBLE, partner, tag, MPI_COMM_WORLD); 
                    MPI_Recv(&recv_sum, 1, MPI_DOUBLE, partner, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    local_sum += recv_sum;
                }
            } else {
                int partner = menum - (1 << i);
                if (partner >= 0) {
                    MPI_Recv(&recv_sum, 1, MPI_DOUBLE, partner, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    MPI_Send(&local_sum, 1, MPI_DOUBLE, partner, tag, MPI_COMM_WORLD); 
                }
            }
        }
    }

    if (menum == 0) {
        *global_sum = local_sum;
    }
}
