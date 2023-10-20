#include <stdio.h>
#include <mpi.h>

void first_strategy(int menum, int nproc, double local_sum, double *global_sum) {

    if (menum == 0) {
        int i;
        for (i = 1; i < nproc; i++) {
            double recv_sum;
            int tag = 10 + i;
            MPI_Recv(&recv_sum, 1, MPI_DOUBLE, i, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            local_sum += recv_sum;
            printf("Sono 0 e ricevo da %d, local_sum: %lf, recv_sum: %lf\n", i, local_sum, recv_sum);
        }
        *global_sum = local_sum;
    } else {
        int tag = 10 + menum;
        MPI_Send(&local_sum, 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);
        printf("Sono %d e spedisco a 0, local_sum: %lf\n", menum, local_sum);
    }
}


void second_strategy(int menum, int nproc, double local_sum, double *global_sum, double log2_nproc) {

    int i;
    for (i = 0; i < log2_nproc; i++) {
        if ((menum % (1 << i)) == 0) {
            int tag = 20 + i;
            double recv_sum;
            if ((menum % (1 << (i + 1))) == 0) {
                int partner = menum + (1 << i);
                if (partner < nproc) {
                    MPI_Recv(&recv_sum, 1, MPI_DOUBLE, partner, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    local_sum += recv_sum;
                    printf("Sono %d e ricevo da %d, local_sum: %lf, recv_sum: %lf\n", menum, partner, local_sum, recv_sum);
                }
            } else {
                int partner = menum - (1 << i);
                if (partner >= 0) {
                    MPI_Send(&local_sum, 1, MPI_DOUBLE, partner, tag, MPI_COMM_WORLD);
                    printf("Sono %d e spedisco a %d, local_sum: %lf\n", menum, partner, local_sum);
                }
            }
        }
    }

    if (menum == 0) {
        *global_sum = local_sum;
    }
}



void third_strategy(int menum, int nproc, double local_sum, double *global_sum, double log2_nproc) {

    int i;
    for (i = 0; i < log2_nproc; i++) {
        int tag = 30 + i;
        double recv_sum;
        if ((menum % (1 << (i + 1))) == 0) {
            int partner = menum + (1 << i);
            if (partner < nproc) {
                MPI_Send(&local_sum, 1, MPI_DOUBLE, partner, tag, MPI_COMM_WORLD);
                MPI_Recv(&recv_sum, 1, MPI_DOUBLE, partner, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                local_sum += recv_sum;
                printf("Sono %d, spedisco a e ricevo da %d, local_sum: %lf, recv_sum: %lf\n", menum, partner, local_sum, recv_sum);
            }
        } else {
            int partner = menum - (1 << i);
            if (partner >= 0) {
                MPI_Recv(&recv_sum, 1, MPI_DOUBLE, partner, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Send(&local_sum, 1, MPI_DOUBLE, partner, tag, MPI_COMM_WORLD);
                local_sum += recv_sum;
                printf("Sono %d e ricevo da e spedisco a %d, local_sum: %lf\n", menum, partner, local_sum);
            }
        }
    }

    if (menum == 0) {
        *global_sum = local_sum;
    }
}
