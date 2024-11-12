/* tempo1.c */

#include <stdio.h>
#include <stdlib.h>
#include "smpl.h"

#define test 1

typedef struct {
   int id;
} TipoProcesso;

TipoProcesso *processo;

int main(int argc, char *argv[]) {
    static int N, token, event, r, i, next;
    static char fa_name[5];

    if (argc != 2) {
        puts("Uso correto: tempo <num-processos>");
        exit(1);
    }

    N = atoi(argv[1]);
    smpl(0, "Simulação de Testes");

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
                next = (token + 1) % N;
                r = status(processo[next].id);
                if (r == 0)
                    printf("O processo %d testou o processo %d correto no tempo %g\n", token, next, time());
                else
                    printf("O processo %d testou o processo %d falho no tempo %g\n", token, next, time());
                schedule(test, 30.0, token);
                break;
        }
    }

    free(processo);
    return 0;
}
