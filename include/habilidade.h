#ifndef HABILIDADE_H
#define HABILIDADE_H

/* habilidade.h - Tabela de ponteiros de função que liga cada raça à sua habilidade. */

#include "partida.h"
#include <stdbool.h>

typedef bool (*Habilidade)(Partida *partida, Cachorro *cachorro);   // true se agiu, false se esperando alvo

bool habilidade_usar(Partida *partida, Cachorro *cachorro);

#endif
