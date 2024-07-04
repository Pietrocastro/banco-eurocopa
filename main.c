#include "TARVBS/TARVBS.c"


void printa_jogador(Jogador jogador) {
    printf("Seleção: %s\n", jogador.selecao);
    printf("ID: %d\n", jogador.id);
    printf("Camisa: %d\n", jogador.camisa);
    printf("Posição: %s\n", jogador.pos);
    printf("Nome: %s\n", jogador.nome);
    printf("Capitão: %s\n", jogador.capitao ? "Sim" : "Não");
    printf("Nascimento: %d de %s de %d\n", jogador.nascimento.dia, jogador.nascimento.mes, jogador.nascimento.ano);
    printf("Idade: %d\n", jogador.idade);
    printf("Jogos: %d\n", jogador.jogos);
    printf("Gols: %d\n", jogador.gols);
    printf("País: %s\n", jogador.pais);
    printf("Time: %s\n", jogador.time);
}


void printTARVBS(char *narq, int t) {
    TARVBS *a = TARVBS_nova(t);
    FILE *f = fopen(narq, "rb");
    fread(a, sizeof(TARVBS), 1, f);

    if (a != NULL) {
        printf("\nArquivo: %s\n", a->narq);
        printf("Numero de chaves: %d\n", a->nchaves);
        printf("Folha: %s\n", a->folha ? "Sim" : "Não");
        printf("Chaves:\n");
        for (int i = 0; i < a->nchaves; i++) {
            printf("\tJogador %d: %s\n", i+1, a->chave[i].nome);
            printf("\n");
        }
        if (!a->folha) {
            for (int i = 0; i <= a->nchaves; i++) {
                printf("\tFilho %d:\n", i);
                printTARVBS(a->filho[i], t);
            }
        }
    }
    else printf("\nERROR\n");
}

int main(){
Jogador jogador1 = {
    .selecao = "Brasil",
    .id = 1,
    .camisa = 10,
    .pos = "AT",
    .nome = "Neymar Jr.",
    .capitao = true,
    .nascimento = {5, "Fevereiro", 1992},
    .idade = 32,
    .jogos = 112,
    .gols = 64,
    .pais = "Brasil",
    .time = "Paris Saint-Germain"
};

Jogador jogador2 = {
    .selecao = "Portugal",
    .id = 2,
    .camisa = 7,
    .pos = "AT",
    .nome = "Cristiano Ronaldo",
    .capitao = true,
    .nascimento = {5, "Fevereiro", 1985},
    .idade = 39,
    .jogos = 184,
    .gols = 115,
    .pais = "Portugal",
    .time = "Al Nassr"
};

Jogador jogador3 = {
    .selecao = "Argentina",
    .id = 3,
    .camisa = 10,
    .pos = "AT",
    .nome = "Lionel Messi",
    .capitao = true,
    .nascimento = {24, "Junho", 1987},
    .idade = 36,
    .jogos = 174,
    .gols = 102,
    .pais = "Argentina",
    .time = "Inter Miami"
};

Jogador jogador4 = {
    .selecao = "França",
    .id = 4,
    .camisa = 10,
    .pos = "AT",
    .nome = "Kylian Mbappe",
    .capitao = false,
    .nascimento = {20, "Dezembro", 1998},
    .idade = 25,
    .jogos = 66,
    .gols = 38,
    .pais = "França",
    .time = "Paris Saint-Germain"
};

Jogador jogador5 = {
    .selecao = "Bélgica",
    .id = 5,
    .camisa = 7,
    .pos = "ME",
    .nome = "Kevin De Bruyne",
    .capitao = true,
    .nascimento = {28, "Junho", 1991},
    .idade = 32,
    .jogos = 93,
    .gols = 24,
    .pais = "Bélgica",
    .time = "Manchester City"
};

    
    FILE *f = TARVBS_cria(3);

    // PROBLEMA NA INSERÇÃO DO 3º JOGADOR EM DIANTE
    // O 3º JOGADOR SOBREESCREVE O 2º PRA SEMPRE
    TARVBS_insere("arqvs/arquivo1.bin", jogador1, 3);

    TARVBS *A = TARVBS_nova(3);
    fread(A, sizeof(TARVBS), 1, f);

    
    printTARVBS("arqvs/arquivo1.bin", 3);
    return 0;
}


// criar a arvore b+ dos arqvs binarios

// criar 11 tabelas hash ordenadas (em ms) cada uma por um parametro diferente e com a informação do endereço de cada jogador na árvore

// fazer as funções de busca de preferencia em um arquivo separado apenas para essas funções de busca