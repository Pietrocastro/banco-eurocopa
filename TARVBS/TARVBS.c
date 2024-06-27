#include "TARVBS.h"

char * id() {
    static int counter = 0;
    counter++;
    
    char *id_str = malloc(20);
    if (id_str == NULL) {
        return NULL;
    }
    sprintf(id_str, "%d", counter);
    return id_str;
}

FILE * TARVBS_cria(char *nome_arq, int t){
    FILE *novo = fopen(strcat(nome_arq, id()), "wb");

    TARVBS *a = (TARVBS *) malloc(sizeof(TARVBS));
    a -> nchaves = 0;
    a -> chave = (Jogador*) malloc(sizeof(Jogador *)*((t*2)-1));
    a -> folha = 1;
    a -> filho = (TARVBS**) malloc(sizeof(TARVBS *) * t*2);
    for (int i = 0; i < (t*2); i++) a -> filho[i] = NULL;

    fwrite(a, sizeof(TARVBS), 1, novo);
    free(a);
    return novo;
}

/*
void TARVBS_libera(char *nome_raiz){
TARVBS *a = (TARVBS *) malloc(sizeof(TARVBS));
FILE *f = 
fread(a, sizeof(TARVBS), 1, f);
if(a){
if(!a->folha){
int i;
for(i = 0; i <= a->nchaves; i++) TARVBS_Libera(a->filho[i]);
}
free(a->chave);
free(a->filho);
free(a);
return NULL;
}
} 
*/

FILE *TARVBS_busca(char *nome_raiz, int id) {
    FILE *f = fopen(nome_raiz, "rb");
    TARVBS * aux = (TARVBS *) malloc(sizeof(TARVBS));
    fread(aux, sizeof(TARVBS), 1, f);

    if (!aux) return NULL;
    int i = 0;
    while (i < aux -> nchaves && id > aux -> chave[i].id) i++;
    if (i < aux -> nchaves && id == aux -> chave[i].id) free(aux); return f;
    if (aux -> folha) free(aux); return NULL;
    char * nome_filho = strcpy(nome_filho, aux -> filho[i]);
    free(aux);
    return TARVBS_busca(nome_filho, id);
}

/*
    for (int i = 0; i < aux -> nchaves; i++) if (aux -> chave[i].id == id) return f;
    return NULL;
*/



FILE * TARVBS_insere(char *nome_raiz, Jogador jogador, int t){
    FILE *f = fopen(nome_raiz, "rb");
    TARVBS *a = (TARVBS *) malloc(sizeof(TARVBS));

    if (TARVBS_busca(nome_raiz, jogador.id)) return f;

    if (!a){
        a = TARVBS_cria(nome_raiz, t);
        a -> chave[0] = jogador;
        a -> nchaves = 1;
        fwrite(a, sizeof(jogador), 1, f);
        free(a);
        return f;
    }

    if (a -> nchaves == (2 * t) - 1){
        FILE *aux_arq;
        TARVBS *aux = TARVBS_cria("arquivo", t);
        aux -> nchaves = 0;
        aux -> folha = 0;
        aux -> filho[0] = a;
        fwrite(aux, sizeof(TARVBS), 1, aux_arq);
        divisao(aux_arq, 1, a, t);
        insere_nao_completo(aux_arq, jogador, t);
        free(a);
        return aux;
    }
    insere_nao_completo(f, jogador.id, t);
    free(a);
    return f;
}

void TARVBS_retira(char *nome_raiz, Jogador jogador, int t);
/*
TARVB *Divisao(TARVB *x, int i, TARVB* y, int t){
    TARVB *z=TARVB_Cria(t);
    z->nchaves= t - 1;
    z->folha = y->folha;
    int j;
    for(j=0;j<t-1;j++) z->chave[j] = y->chave[j+t];
    if(!y->folha){
        for(j=0;j<t;j++){
            z->filho[j] = y->filho[j+t];
            y->filho[j+t] = NULL;
        }
    }
    y->nchaves = t-1;
    for(j=x->nchaves; j>=i; j--) x->filho[j+1]=x->filho[j];
    x->filho[i] = z;
    for(j=x->nchaves; j>=i; j--) x->chave[j] = x->chave[j-1];
    x->chave[i-1] = y->chave[t-1];
    x->nchaves++;
    return x;
}


TARVB *Insere_Nao_Completo(TARVB *x, int k, int t){
    int i = x->nchaves-1;
    if(x->folha){
        while((i>=0) && (k<x->chave[i])){
            x->chave[i+1] = x->chave[i];
            i--;
        }
        x->chave[i+1] = k;
        x->nchaves++;
        return x;
    }
    while((i>=0) && (k<x->chave[i])) i--;
    i++;
    if(x->filho[i]->nchaves == ((2*t)-1)){
        x = Divisao(x, (i+1), x->filho[i], t);
        if(k>x->chave[i]) i++;
    }
    x->filho[i] = Insere_Nao_Completo(x->filho[i], k, t);
    return x;
}


TARVB *TARVB_Insere(TARVB *T, int k, int t){
    if(TARVB_Busca(T,k)) return T;
    if(!T){
        T=TARVB_Cria(t);
        T->chave[0] = k;
        T->nchaves=1;
        return T;
    }
    if(T->nchaves == (2*t)-1){
        TARVB *S = TARVB_Cria(t);
        S->nchaves=0;
        S->folha = 0;
        S->filho[0] = T;
        S = Divisao(S,1,T,t);
        S = Insere_Nao_Completo(S,k,t);
        return S;
    }
    T = Insere_Nao_Completo(T,k,t);
    return T;
}
*/