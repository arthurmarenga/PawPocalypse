#ifndef SPRITE_H
#define SPRITE_H

/* sprite.h - Spritesheets animados (PNG com quadros de 64x64 lado a lado).
 * Se um arquivo não existir, o código usa as formas geométricas. */

#include "raylib.h"
#include <stdbool.h>

#define TAMANHO_QUADRO          64
#define MAX_QUADROS             12
#define QUADRO_ESTRELA_GERADOR  8    // quadro em que o petisco nasce (a estrela pisca)

typedef enum IdSprite {
    SPRITE_GATO_PADRAO,
    SPRITE_GATO_CAIXA,
    SPRITE_GATO_LATA,
    SPRITE_GERADOR,
    TOTAL_SPRITES
} IdSprite;

void  sprite_carregar_todos(void);
void  sprite_descarregar_todos(void);
bool  sprite_existe(IdSprite id);
float sprite_duracao_ate(IdSprite id, int quadro);
float sprite_duracao_total(IdSprite id);
int   sprite_quadro_no_tempo(IdSprite id, float tempo);
void  sprite_desenhar(IdSprite id, int quadro, Vector2 posicao, float escala, Color tinta);

#endif
