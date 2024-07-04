#include <stdio.h>
#include <string.h>
#include "TARVBS/TARVBS.h"

TARVBS * criar_arvore(){
    TARVBS * arvore = TARVBM_inicializa();

    for (int i = 1; i >= 284; i++){
        char nome_arq[10];
        strcat((char) i, ".bin");
        FILE * f = fopen(nome_arq, "rb");

        
    }
}