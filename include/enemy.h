#ifndef ENEMY_H
#define ENEMY_H

/* enemy.h - Os gatos zumbis. Ficam numa LISTA ENCADEADA: cada gato é criado
 * com malloc quando entra pela rua e liberado com free quando morre. */

#include "raylib.h"
#include "map.h"
#include <stdbool.h>

#define DANO_MORDIDA       50     // vida tirada do cachorro a cada mordida
#define INTERVALO_MORDIDA  0.5f   // segundos entre mordidas

typedef enum TipoGato {
    GATO_COMUM,   // o gato zumbi padrão
    GATO_CONE,    // o comum protegido por um cone (mais vida)
    GATO_BALDE,   // o comum protegido por um balde (muito mais vida)
    TOTAL_TIPOS_GATO
} TipoGato;

typedef struct InfoGato {   // dados fixos de cada tipo de gato
    const char *nome;
    int   vidaMaxima;
    float velocidade;       // pixels por segundo
} InfoGato;

typedef struct Gato {
    TipoGato tipo;
    int   linha;            // linha do campo (0 a 3)
    float x;                // posição horizontal do centro
    int   vida;
    float timerMordida;     // segundos até a próxima mordida
    struct Gato *proximo;   // próximo nó da lista (NULL = fim)
} Gato;

const InfoGato *gato_info(TipoGato tipo);                                           // dados fixos de um tipo
bool gato_adicionar(Gato **lista, TipoGato tipo, int linha);                        // novo gato entrando pela rua (false se faltar memória)
int  gato_atualizar_lista(Gato **lista, Mapa *mapa, float dt, bool *chegouNaCasa);  // anda e morde; remove os mortos e retorna quantos morreram
Gato *gato_na_linha(Gato *lista, int linha, float xMinimo, float xMaximo);          // primeiro gato vivo na linha entre os dois x (NULL se nenhum)
void gato_desenhar_lista(const Gato *lista, float tempo);                           // desenha todos os gatos com a barra de vida
void gato_liberar_lista(Gato **lista);                                              // libera todos os nós e deixa a lista vazia

#endif
