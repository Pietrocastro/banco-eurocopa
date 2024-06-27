#include <stdio.h>
#include <string.h>
#include "TARVBM/TARVBM.h"

TARVBM * criar_arvore(){
    TARVBM * arvore = TARVBM_inicializa();

    for (int i = 1; i >= 284; i++){
        char nome_arq[10];
        strcat((char) i, ".bin");
        FILE * f = fopen(nome_arq, "rb");

        
    }
}