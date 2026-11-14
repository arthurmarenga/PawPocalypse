#ifndef HORDA_H
#define HORDA_H

#include "enemy.h"
#include "save.h"
#include <stdbool.h>

#define HORDAS_POR_FASE  4
#define MAX_GATOS_HORDA  40

typedef struct Horda {
    int      atual;
    bool     esperando;
    float    timer;
    float    timerAnuncio;
    TipoGato fila[MAX_GATOS_HORDA];
    int      total;
    int      proximo;
} Horda;

void  horda_iniciar(Horda *horda);
void  horda_atualizar(Horda *horda, int fase, Gato **gatos, float dt);
bool  horda_terminou(const Horda *horda);
float horda_progresso(const Horda *horda);
void  horda_desenhar(const Horda *horda);

#endif
