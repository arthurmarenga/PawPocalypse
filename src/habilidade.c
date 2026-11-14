/* habilidade.c - Habilidades das raças: tabela de ponteiros de função. */

#include "habilidade.h"

#include <stdlib.h>

#define ALCANCE_PASTOR  (1.5f * TAMANHO_CELULA)
#define RAIO_3X3        (1.5f * TAMANHO_CELULA)
#define ALCANCE_SHIBA   (2.0f * TAMANHO_CELULA)
#define EMPURRAO_SHIBA  48.0f

static const Color COR_EXPLOSAO = { 255, 150,  40, 255 };
static const Color COR_CURA     = { 130, 230, 120, 255 };
static const Color COR_SUSTO    = { 255, 200,  80, 255 };

static Vector2 centro_do(const Cachorro *cachorro)
{
    return mapa_centro_celula(cachorro->linha, cachorro->coluna);
}

/* 'recuo' afasta a bolinha alguns pixels da boca para o disparo duplo. */
static void lancar_bolinha(Partida *partida, const Cachorro *cachorro, int linha, float recuo, bool gelada)
{
    Vector2 centro = mapa_centro_celula(linha, cachorro->coluna);
    Vector2 boca = { centro.x + 16 - recuo, centro.y - 4 };
    projetil_adicionar(&partida->projeteis, boca, linha, cachorro_info(cachorro->tipo)->dano, gelada);
}

static bool produzir_petisco(Partida *partida, Cachorro *cachorro)
{
    Vector2 centro = centro_do(cachorro);
    petisco_adicionar(&partida->petiscos, (Vector2){ centro.x + 18 + rand() % 7 - 3, centro.y + 6 }, centro.y + 20);
    return true;
}

static bool atirar(Partida *partida, Cachorro *cachorro)
{
    if (!gato_na_mira(partida->gatos, cachorro->linha, centro_do(cachorro).x)) return false;
    lancar_bolinha(partida, cachorro, cachorro->linha, 0, false);
    return true;
}

static bool atirar_duplo(Partida *partida, Cachorro *cachorro)
{
    if (!atirar(partida, cachorro)) return false;
    lancar_bolinha(partida, cachorro, cachorro->linha, 14, false);
    return true;
}

static bool atirar_gelo(Partida *partida, Cachorro *cachorro)
{
    if (!gato_na_mira(partida->gatos, cachorro->linha, centro_do(cachorro).x)) return false;
    lancar_bolinha(partida, cachorro, cachorro->linha, 0, true);
    return true;
}

/* Atira nas três linhas se houver gato em qualquer uma delas. */
static bool atirar_tres_linhas(Partida *partida, Cachorro *cachorro)
{
    float x = centro_do(cachorro).x;
    bool temAlvo = false;
    for (int l = cachorro->linha - 1; l <= cachorro->linha + 1; l++) temAlvo = temAlvo || gato_na_mira(partida->gatos, l, x);
    if (!temAlvo) return false;

    for (int l = cachorro->linha - 1; l <= cachorro->linha + 1; l++) {
        if (l >= 0 && l < LINHAS_MAPA) lancar_bolinha(partida, cachorro, l, 0, false);
    }
    return true;
}

static bool morder(Partida *partida, Cachorro *cachorro)
{
    float x = centro_do(cachorro).x;
    return gato_ferir_area(partida->gatos, cachorro->linha, x - RAIO_GATO, x + ALCANCE_PASTOR, cachorro_info(cachorro->tipo)->dano) > 0;
}

/* vida = 0 faz o mapa remover o Westie após a explosão. */
static bool explodir_mina(Partida *partida, Cachorro *cachorro)
{
    Rectangle celula = mapa_retangulo_celula(cachorro->linha, cachorro->coluna);
    float xMax = celula.x + celula.width + RAIO_GATO;
    if (gato_ferir_area(partida->gatos, cachorro->linha, celula.x, xMax, cachorro_info(cachorro->tipo)->dano) == 0) return false;

    cachorro->vida = 0;
    partida_adicionar_efeito(partida, centro_do(cachorro), TAMANHO_CELULA * 0.6f, COR_EXPLOSAO);
    return true;
}

static bool explodir_area(Partida *partida, Cachorro *cachorro)
{
    Vector2 centro = centro_do(cachorro);
    for (int l = cachorro->linha - 1; l <= cachorro->linha + 1; l++) {
        gato_ferir_area(partida->gatos, l, centro.x - RAIO_3X3, centro.x + RAIO_3X3, cachorro_info(cachorro->tipo)->dano);
    }
    cachorro->vida = 0;
    partida_adicionar_efeito(partida, centro, RAIO_3X3, COR_EXPLOSAO);
    return true;
}

static bool curar_vizinhos(Partida *partida, Cachorro *cachorro)
{
    int cura = cachorro_info(cachorro->tipo)->dano;
    for (int l = cachorro->linha - 1; l <= cachorro->linha + 1; l++) {
        for (int c = cachorro->coluna - 1; c <= cachorro->coluna + 1; c++) {
            if (l < 0 || l >= LINHAS_MAPA || c < 0 || c >= COLUNAS_MAPA) continue;
            Cachorro *vizinho = partida->mapa.grade[l][c];
            if (vizinho == NULL || vizinho == cachorro) continue;

            int maxima = cachorro_info(vizinho->tipo)->vidaMaxima;
            vizinho->vida = (vizinho->vida + cura > maxima) ? maxima : vizinho->vida + cura;
        }
    }
    partida_adicionar_efeito(partida, centro_do(cachorro), RAIO_3X3, COR_CURA);
    return true;
}

static bool assustar(Partida *partida, Cachorro *cachorro)
{
    Vector2 centro = centro_do(cachorro);
    if (gato_empurrar_area(partida->gatos, cachorro->linha, centro.x, centro.x + ALCANCE_SHIBA, EMPURRAO_SHIBA) == 0) return false;
    partida_adicionar_efeito(partida, centro, ALCANCE_SHIBA * 0.5f, COR_SUSTO);
    return true;
}

static const Habilidade habilidades[TOTAL_TIPOS_CACHORRO] = {
    [TIPO_POTE_RACAO]    = produzir_petisco,
    [TIPO_BEAGLE]        = atirar,
    [TIPO_BULLDOG]       = NULL,   // muralha: só aguenta as mordidas
    [TIPO_DALMATA]       = atirar_duplo,
    [TIPO_HUSKY]         = atirar_gelo,
    [TIPO_PASTOR_ALEMAO] = morder,
    [TIPO_WESTIE]        = explodir_mina,
    [TIPO_SAO_BERNARDO]  = curar_vizinhos,
    [TIPO_DACHSHUND]     = atirar_tres_linhas,
    [TIPO_ROTTWEILER]    = explodir_area,
    [TIPO_SHIBA_INU]     = assustar,
};

bool habilidade_usar(Partida *partida, Cachorro *cachorro)
{
    Habilidade habilidade = habilidades[cachorro->tipo];
    return habilidade != NULL && habilidade(partida, cachorro);
}
