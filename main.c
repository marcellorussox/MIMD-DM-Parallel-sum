#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <mpi.h>
#include "strategies.h"
#include "analytics.h"

#define CSV_OUTPUT "output/analytics_data.csv" 

int main(int argc, char **argv) {
    int menum, nproc, strategy;
    double *numbers, local_sum = 0.0, global_sum = 0.0;
    int i;

    double t0, t1, dt; 
    double timetot;


    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &menum);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);

    if (argc < 3) {
        if (menum == 0) {
            printf("Utilizzo: %s [strategia: 1, 2, 3] <N> [numeri ...]\n", argv[0]);
        }
        MPI_Finalize();
        return 1;
    }

    strategy = atoi(argv[1]);
    int N = atoi(argv[2]);

    if (N < 0) {
        if (menum == 0) {
            printf("N deve essere un numero positivo intero.\n");
        }
        MPI_Finalize();
        return 1;
    }

    if (N <= 20) {
        if (argc < 3 + N) {
            if (menum == 0) {
                printf("Non hai inserito abbastanza numeri.\n");
            }
            MPI_Finalize();
            return 1;
        }

        if (menum == 0) {
            numbers = (double *)malloc(N * sizeof(double));
            for (i = 0; i < N; i++) {
                numbers[i] = atof(argv[3 + i]);
            }
        }
    } else {
        if (menum == 0) {
            srand(time(NULL));
            numbers = (double *)malloc(N * sizeof(double));
            for (i = 0; i < N; i++) {
                numbers[i] = ((double)rand() / RAND_MAX);
            }
        }
    }

    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int nloc = N / nproc;
    int rest = N % nproc;
    nloc = (menum < rest) ? (nloc + 1) : nloc;
    double *local_numbers = (double *)malloc(nloc * sizeof(double));

    if (menum == 0) {

        int offset = 0;
        int dest;
        int dest_count = nloc;

        for (dest = 1; dest < nproc; dest++) {
            offset += dest_count;
            int tag = dest;
            dest_count = (dest == rest) ? (dest_count - 1) : dest_count;
            MPI_Send(&numbers[offset], dest_count, MPI_DOUBLE, dest, tag, MPI_COMM_WORLD);
        }

        for (i = 0; i < nloc; i++) {
            local_numbers[i] = numbers[i];
        }
    } else {
        int tag = menum;
        MPI_Recv(local_numbers, nloc, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    t0=MPI_Wtime();

    for (i = 0; i < nloc; i++) {
        local_sum += local_numbers[i];
    }

    double log2_nproc = log(nproc) / log(2);

    if (log2_nproc != (int)log2_nproc) {
        if (menum == 0) {
            printf("Il numero di processi non è una potenza di due. Sarà utilizzata la prima strategia.\n");
        }
        strategy = 1;
    }

    if (strategy == 1)
        first_strategy(menum, nproc, local_sum, &global_sum);
    else if (strategy == 2)
        second_strategy(menum, nproc, local_sum, &global_sum, log2_nproc);
    else if (strategy == 3)
        third_strategy(menum, nproc, local_sum, &global_sum, log2_nproc);

    t1=MPI_Wtime();
    dt=t1-t0;

    MPI_Reduce(&dt,&timetot,1,MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);


    if (menum == 0) {
        printf("Somma totale: %lf\n", global_sum);
        printf("Tempo totale impiegato: %lf secondi\n", timetot);
        export_data_csv(CSV_OUTPUT, nproc, N, strategy, timetot);
    }

    MPI_Finalize();
    if (menum == 0) {
        free(numbers);
    }
    free(local_numbers);


    return 0;
}
