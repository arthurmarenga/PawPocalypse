#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include "raylib.h"
#include <stdbool.h>

#define TOTAL_RECORDES     10
#define TAMANHO_NOME       12
#define ARQUIVO_RECORDES   "assets/data/highscores.txt"

typedef struct Recorde {
    char  nome[TAMANHO_NOME + 1];
    int   fase;
    int   pontos;
    float tempo;
} Recorde;

typedef struct Ranking {
    Recorde lista[TOTAL_RECORDES];
    int     total;
} Ranking;

void ranking_carregar(Ranking *ranking);
bool ranking_gravar(const Ranking *ranking);
int  ranking_posicao(const Ranking *ranking, int pontos);
int  ranking_inserir(Ranking *ranking, const char *nome, int fase, int pontos, float tempo);
void ranking_desenhar(const Ranking *ranking, Rectangle area);

#endif
