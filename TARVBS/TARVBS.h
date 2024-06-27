#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int boolean;

#define true 1
#define false 0

typedef struct jogador {
    int id;
    int numero_camisa;
    char posicao[3];
    char nome[50];
    char data_nascimento[20];
    int partidas_selecao;
    int gols_selecao;
    char pais_time_atual[50];
    char nome_time_atual[50];
} Jogador;

typedef struct arvbms {
    int nchaves;
    Jogador *chave;
    boolean folha;
    char **filho;
}TARVBS;

// Funções

FILE * TARVBS_cria(char *nome_arq, int t);
FILE * TARVBS_busca(char *nome_raiz, int n);
void TARVBS_insere(char *nome_raiz, Jogador jogador, int t);
void TARVBS_retira(char *nome_raiz, Jogador jogador, int t);
