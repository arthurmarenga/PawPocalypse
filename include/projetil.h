#ifndef PROJETIL_H
#define PROJETIL_H

/* projetil.h - Latidos disparados pelos cachorros. Ficam numa LISTA
 * ENCADEADA: cada latido é criado com malloc e liberado com free quando
 * acerta um gato ou sai da tela. */

#include "enemy.h"
#include <stdbool.h>

#define VELOCIDADE_LATIDO 200.0f   // pixels por segundo

typedef struct Projetil {
    int   linha;                 // linha do campo onde ele viaja
    float x;                     // posição horizontal
    int   dano;                  // vida tirada do gato atingido
    struct Projetil *proximo;    // próximo nó da lista (NULL = fim)
} Projetil;

bool projetil_adicionar(Projetil **lista, int linha, float x, int dano);  // novo latido no início da lista (false se faltar memória)
void projetil_atualizar_lista(Projetil **lista, Gato *gatos, float dt);   // move, fere o gato atingido e remove os que acertaram ou saíram
void projetil_desenhar_lista(const Projetil *lista);                      // desenha os latidos como ondas de som
void projetil_liberar_lista(Projetil **lista);                            // libera todos os nós e deixa a lista vazia

#endif
