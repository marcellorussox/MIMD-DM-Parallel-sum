#include <stdio.h>
#include <stdlib.h>


void export_data_csv(char* nome_file, int nproc, int ninput, int strategy, double time) {
    FILE* file = fopen(nome_file, "a");

    if (file == NULL) {
        fprintf(stderr, "Errore nell'apertura del file\n");
        return;
    }

    fprintf(file, "%d,%d,%d,%lf\n", nproc, ninput, strategy, time);
    fclose(file);
}