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

void sobrescrever(FILE *f, TARVBS *a) {
    fseek(f, 0, SEEK_SET);  // Posiciona no início do arquivo
    fwrite(a, sizeof(TARVBS), 1, f);  // Escreve a estrutura TARVBS no arquivo
}

TARVBS* TARVBS_nova(int t) {
    TARVBS *nova = (TARVBS *)malloc(sizeof(TARVBS));
    if (!nova) {
        perror("Erro ao alocar memória para TARVBS");
        return NULL;
    }

    nova->chave = (Jogador *)malloc(sizeof(Jogador) * (2 * t - 1));
    if (!nova->chave) {
        perror("Erro ao alocar memória para chaves de TARVBS");
        free(nova);
        return NULL;
    }

    nova->filho = (char **)malloc(sizeof(char *) * (2 * t));
    if (!nova->filho) {
        perror("Erro ao alocar memória para filhos de TARVBS");
        free(nova->chave);
        free(nova);
        return NULL;
    }

    nova->nchaves = 0;
    nova->folha = 1;

    return nova;
}

void freeABS(TARVBS *a, int t) {
    if (a != NULL) {
        if (!a->folha) {
            for (int i = 0; i < 2 * t; i++) {
                if (a->filho[i] != NULL) {
                    free(a->filho[i]);
                }
            }
            free(a->filho);
        }
        free(a->chave);
        free(a);
    }
}

FILE * TARVBS_cria(int t) { 
    char nome[21] = "arqvs/arquivo";
    strcat(nome, id());
    strcat(nome, ".bin");
    FILE *f = fopen(nome, "wb");
    TARVBS *a = TARVBS_nova(t);
    strcpy(a -> narq, nome);
    fwrite(a, sizeof(TARVBS), 1, f);
    fclose(f);
    f = fopen(nome, "rb+");
    return f;
}

char *TARVBS_busca(char *nome_raiz, int id, int t) {
    FILE *f = fopen(nome_raiz, "rb");
    if (!f) {
        perror("Erro ao abrir arquivo");
        return NULL;
    }

    TARVBS *aux = TARVBS_nova(t);
    if (!aux) {
        fclose(f);
        perror("Erro ao alocar memória para TARVBS");
        return NULL;
    }

    fread(aux, sizeof(TARVBS), 1, f);
    fclose(f);

    int i = 0;
    while (i < aux->nchaves && id > aux->chave[i].id) {
        i++;
    }

    if (i < aux->nchaves && id == aux->chave[i].id) {
        char *nome_arquivo = strdup(aux->narq);
        freeABS(aux, t);
        return nome_arquivo;
    }

    if (aux->folha) {
        freeABS(aux, t);
        return NULL;
    }

    char *nome_filho = strdup(aux->filho[i]);
    freeABS(aux, t);
    return TARVBS_busca(nome_filho, id, t);
}

void divisao(FILE *f_x, int i, FILE *f_y, int t) {
    TARVBS *x = TARVBS_nova(t);
    TARVBS *y = TARVBS_nova(t);
    fseek(f_x, 0, SEEK_SET);
    fread(x, sizeof(TARVBS), 1, f_x);
    fseek(f_y, 0, SEEK_SET);
    fread(y, sizeof(TARVBS), 1, f_y);

    FILE *f_z = TARVBS_cria(t);
    TARVBS *z = TARVBS_nova(t);
    fread(z, sizeof(TARVBS), 1, f_z);


    z->nchaves = t - 1;
    z->folha = y->folha;
    for (int j = 0; j < t - 1; j++) {
        z->chave[j] = y->chave[j + t]; // passa as chaves maiores de y para z
    }
    if (!y->folha) {
        for (int j = 0; j < t; j++) {
            strcpy(z->filho[j], y->filho[j + t]);
            y->filho[j + t] = NULL;
        } // se y tiver filhos, passa os maiores para z
    }
    y->nchaves = t - 1;

    for (int j = x->nchaves; j >= i; j--) {
        strcpy(x->filho[j + 1], x->filho[j]);
    } // atualiza as chaves de x

    sobrescrever(f_z, z);
    fclose(f_z);
    
    char z_narq[21];
    strcpy(z_narq, z->narq);
    strcpy(x->filho[i], z_narq);  // atualiza o nome dos filhos de x

    for (int j = x->nchaves; j >= i; j--) {
        x->chave[j] = x->chave[j - 1];
    }
    x->chave[i - 1] = y->chave[t - 1];
    x->nchaves++;

    sobrescrever(f_x, x);
    sobrescrever(f_y, y);

    freeABS(x, t);
    freeABS(y, t);
    freeABS(z, t);
}


void insere_nao_completo(FILE *f_x, Jogador jogador, int t) {
    TARVBS *x = TARVBS_nova(t);
    fseek(f_x, 0, SEEK_SET);
    fread(x, sizeof(TARVBS), 1, f_x);

    int i = x->nchaves - 1;
    if (x->folha) {
        while (i >= 0 && jogador.id < x->chave[i].id) {
            x->chave[i + 1] = x->chave[i];
            i--;
        }
        x->chave[i + 1] = jogador;
        x->nchaves++;
        sobrescrever(f_x, x);
        freeABS(x, t);
        return;
    }

    while (i >= 0 && jogador.id < x->chave[i].id) i--;
    i++;
    FILE *ptr = fopen(x->filho[i], "rb+");
    if (!ptr) {
        perror("Erro ao abrir arquivo de filho");
        freeABS(x, t);
        return;
    }

    TARVBS *aux = TARVBS_nova(t);
    fread(aux, sizeof(TARVBS), 1, ptr);
    if (aux->nchaves == (2 * t) - 1) {
        fclose(ptr);
        divisao(f_x, i + 1, ptr, t);
        fseek(f_x, 0, SEEK_SET);
        fread(x, sizeof(TARVBS), 1, f_x);
        if (jogador.id > x->chave[i].id) i++;
    }

    insere_nao_completo(ptr, jogador, t);
    fclose(ptr);

    sobrescrever(f_x, x);
    freeABS(x, t);
    freeABS(aux, t);
}


void TARVBS_insere(char *nome_raiz, Jogador jogador, int t) {
    if (TARVBS_busca(nome_raiz, jogador.id, t)) return;
    
    FILE *f = fopen(nome_raiz, "rb+");
    
    if (!f) {
        f = TARVBS_cria(t);
    }

    TARVBS *a = TARVBS_nova(t);
    fseek(f, 0, SEEK_SET);
    fread(a, sizeof(TARVBS), 1, f);

    if (a->nchaves == 0) { // Verificar se a árvore está vazia
        strcpy(a -> narq, nome_raiz);
        a->chave[0] = jogador;
        a->nchaves = 1;
        sobrescrever(f, a);
        freeABS(a, t);
        fclose(f);
        return;
    }

    //até aqui ta funcionando

    TARVBS *s = TARVBS_nova(t);
    FILE *f_s;
    if (a->nchaves == (2 * t) - 1) { // Se a raiz estiver cheia
        f_s = TARVBS_cria(t);
        fread(s, sizeof(TARVBS), 1, f_s);

        s->nchaves = 0;
        s->folha = false;
        strcpy(s->filho[0], a->narq);
        
        sobrescrever(f_s, s);

        divisao(f_s, 1, f, t);
        insere_nao_completo(f_s, jogador, t);

        freeABS(a, t);
        freeABS(s, t);
        fclose(f_s);
        fclose(f);
        return;
    }
    freeABS(s, t);

    insere_nao_completo(f, jogador, t);
    sobrescrever(f, a);
    freeABS(a, t);
    fclose(f);
    return;
}

/*
insere:
    1. se tiver na arvore retorna a raiz CHECK
    2. se não existir arvore cria um nó folha CHECK
    3. se tiver o maximo de chaves, cria um nó secundário que vira pai do nó com maximo de chaves CHECK
    4. chama a função divisao para dividir o nó com maximo de chaves CHECK
    5. chama a insere não completo CHECK


divisão
    1. cria um nó z que vai ficar na mesma altura do nó y CHECK
    2. divide y em 2 partes, uma parte continua em y e outra fica em z (parte maior em z) CHECK
    3. se y tiver filhos, passa parte dos filhos para z CHECK
    4. ajusta os filhos do que subiu CHECK
    5. ajusta as chaves do que subiu CHECK


insere nao completo
    1. se x for folha vai percorrendo até achar o lugar certo pra inserir, depois ajusta as outras chaves CHECK
    2. acha o filho que tem que descer para inserir, se o filho tiver o maximo de chaves, divide e sobe a chave do meio para a atual
    3. chama recursivo para o nó filho que tem que descer

*/


void TARVBS_retira(char *nome_raiz, Jogador jogador, int t);





































