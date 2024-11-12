/* tempo_task_7_4.c */

#include <stdio.h>
#include <stdlib.h>
#include "smpl.h"

#define test 1
#define UNKNOWN -1
#define CORRECT 0
#define FAULTY 1

typedef struct {
   int id;
   int state;
   int State[10];
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
    smpl(0, "Simulação com Compartilhamento de Estados");

    processo = (TipoProcesso *) malloc(N * sizeof(TipoProcesso));
    for (i = 0; i < N; i++) {
        sprintf(fa_name, "%d", i);
        processo[i].id = facility(fa_name, 1);
        processo[i].state = CORRECT;
        for (int j = 0; j < N; j++) {
            processo[i].State[j] = (j == i) ? CORRECT : UNKNOWN;
        }
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
                processo[token].State[next] = (r == 0) ? CORRECT : FAULTY;

                if (r == 0) {
                    for (int j = 0; j < N; j++) {
                        if (j != token) {
                            processo[token].State[j] = processo[next].State[j];
                        }
                    }
                    printf("O processo %d compartilhou estados com o processo %d no tempo %g\n", token, next, time());
                }
                
                printf("Vetor de Estados do processo %d: ", token);
                for (int j = 0; j < N; j++) {
                    printf("%d ", processo[token].State[j]);
                }
                printf("\n");

                schedule(test, 30.0, token);
                break;
        }
    }

    free(processo);
    return 0;
}
