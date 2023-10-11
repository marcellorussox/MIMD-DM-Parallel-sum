#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include "strategies.h"
#include "my_math.h"

int main(int argc, char **argv) {
    int menum, nproc, strategy;
    double *numbers, local_sum = 0.0, global_sum = 0.0;

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
            for (int i = 0; i < N; i++) {
                numbers[i] = atof(argv[3 + i]);
            }
        }
    } else {
        if (menum == 0) {
            srand(time(NULL));
            numbers = (double *)malloc(N * sizeof(double));
            for (int i = 0; i < N; i++) {
                numbers[i] = ((double)rand() / RAND_MAX);
                numbers[i] = 1;
            }
        }
    }

    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int nloc = N / nproc;
    int rest = N % nproc;
    nloc = (menum < rest) ? (nloc + 1) : nloc;
    double *local_numbers = (double *)malloc(nloc * sizeof(double));

    MPI_Scatter(numbers, nloc, MPI_DOUBLE, local_numbers, nloc, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for (int i = 0; i < nloc; i++) {
        local_sum += local_numbers[i];
    }

    double log_nproc = log_2(nproc);

    if (log_nproc != (int)log_nproc) {
        if (menum == 0) {
            printf("Il numero di processi non è una potenza di due. Sarà utilizzata la prima strategia.\n");
        }
        strategy = 1;
    }

    if (strategy == 1)
        first_strategy(menum, nproc, local_sum, &global_sum);
    else if (strategy == 2)
        second_strategy(menum, nproc, local_sum, &global_sum);
    else if (strategy == 3)
        third_strategy(menum, nproc, local_sum, &global_sum);

    if (menum == 0) {
        printf("Somma totale: %lf\n", global_sum);
    }
    printf("%d", menum);

    MPI_Finalize();
    if (menum == 0) {
        free(numbers);
    }
    free(local_numbers);

    return 0;
}
