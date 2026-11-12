/* sprite.c - Spritesheets animados: carregamento e desenho dos quadros. */

#include "sprite.h"

#include <math.h>

typedef struct InfoSprite {
    const char *arquivo;
    int     quadros;
    int     duracaoMs[MAX_QUADROS];   // tempo de cada quadro em ms (do LEIAME.txt)
    Vector2 origem;                   // ponto do quadro que fica sobre a posição (pés ou base)
} InfoSprite;

static const InfoSprite infos[TOTAL_SPRITES] = {
    [SPRITE_GATO_PADRAO] = { "assets/sprites/gato_zumbi_padrao/gato_zumbi_padrao_andar_spritesheet.png", 8,
                             { 160, 140, 140, 200, 180, 140, 140, 160 }, { 32, 58 } },
    [SPRITE_GATO_CAIXA]  = { "assets/sprites/gato_zumbi_caixa/gato_zumbi_caixa_andar_spritesheet.png", 8,
                             { 160, 140, 140, 200, 180, 140, 140, 160 }, { 32, 58 } },
    [SPRITE_GATO_LATA]   = { "assets/sprites/gato_zumbi_lata/gato_zumbi_lata_andar_spritesheet.png", 8,
                             { 160, 140, 140, 200, 180, 140, 140, 160 }, { 32, 56 } },
    [SPRITE_GERADOR]     = { "assets/sprites/gerador_petiscos/gerador_petiscos_spritesheet.png", 11,
                             { 300, 140, 140, 120, 120, 100, 100, 80, 160, 100, 120 }, { 23, 62 } },
};

static Texture2D texturas[TOTAL_SPRITES];   // id 0 = não carregada

void sprite_carregar_todos(void)
{
    for (int i = 0; i < TOTAL_SPRITES; i++) {
        texturas[i] = FileExists(infos[i].arquivo) ? LoadTexture(infos[i].arquivo) : (Texture2D){ 0 };
    }
}

void sprite_descarregar_todos(void)
{
    for (int i = 0; i < TOTAL_SPRITES; i++) {
        if (texturas[i].id != 0) UnloadTexture(texturas[i]);
        texturas[i] = (Texture2D){ 0 };
    }
}

bool sprite_existe(IdSprite id)
{
    return texturas[id].id != 0;
}

float sprite_duracao_ate(IdSprite id, int quadro)
{
    int ms = 0;
    for (int i = 0; i < quadro; i++) ms += infos[id].duracaoMs[i];
    return ms / 1000.0f;
}

float sprite_duracao_total(IdSprite id)
{
    return sprite_duracao_ate(id, infos[id].quadros);
}

/* Encontra o quadro correto no loop da animação dado o tempo decorrido. */
int sprite_quadro_no_tempo(IdSprite id, float tempo)
{
    float ms = fmodf(tempo, sprite_duracao_total(id)) * 1000.0f;
    int quadro = 0;
    while (quadro < infos[id].quadros - 1 && ms >= infos[id].duracaoMs[quadro]) {
        ms -= infos[id].duracaoMs[quadro];
        quadro++;
    }
    return quadro;
}

/* Recorta o quadro i (x = i*64) e o desenha ampliado 'escala' vezes com a
 * origem do sprite sobre 'posicao'. WHITE = cores normais. */
void sprite_desenhar(IdSprite id, int quadro, Vector2 posicao, float escala, Color tinta)
{
    Rectangle recorte = { quadro * TAMANHO_QUADRO, 0, TAMANHO_QUADRO, TAMANHO_QUADRO };
    Rectangle destino = { floorf(posicao.x), floorf(posicao.y), TAMANHO_QUADRO * escala, TAMANHO_QUADRO * escala };
    Vector2 origem = { infos[id].origem.x * escala, infos[id].origem.y * escala };
    DrawTexturePro(texturas[id], recorte, destino, origem, 0.0f, tinta);
}
