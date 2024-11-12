/* tempo0.c */

#include <stdio.h>
#include <stdlib.h>
#include "smpl.h"

#define test 1
#define fault 2
#define recovery 3

typedef struct {
   int id;
} TipoProcesso;

TipoProcesso *processo;

int main(int argc, char *argv[]) {
    static int N, token, event, r, i;
    static char fa_name[5];

    if (argc != 2) {
        puts("Uso correto: tempo <num-processos>");
        exit(1);
    }

    N = atoi(argv[1]);
    smpl(0, "Um Exemplo de Simulação");

    processo = (TipoProcesso *) malloc(N * sizeof(TipoProcesso));
    for (i = 0; i < N; i++) {
        sprintf(fa_name, "%d", i);
        processo[i].id = facility(fa_name, 1);
    }

    for (i = 0; i < N; i++) {
        schedule(test, 30.0, i);
    }

    while (time() < 100.0) {
        cause(&event, &token);
        switch (event) {
            case test:
                printf("O processo %d está funcionando no tempo %g\n", token, time());
                schedule(test, 30.0, token);
                break;
        }
    }

    free(processo);
    return 0;
}
