#ifndef STRATEGIES_H
#define STRATEGIES_H

void first_strategy(int menum, int nproc, double local_sum, double *global_sum);
void second_strategy(int menum, int nproc, double local_sum, double *global_sum, double  log2_nproc);
void third_strategy(int menum, int nproc, double local_sum, double *global_sum, double  log2_nproc);

#endif
