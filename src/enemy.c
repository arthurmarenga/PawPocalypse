/* enemy.c - Lista encadeada de gatos zumbis: movimento, mordidas e desenho. */

#include "enemy.h"
#include "config.h"
#include "ui.h"

#include <math.h>
#include <stdlib.h>

#define X_ENTRADA     (LARGURA_VIRTUAL + 16)   // os gatos surgem fora da tela, na rua
#define DISTANCIA_BOCA  14                      // do centro do gato até a boca (lado esquerdo)

static const Color PELO  = { 128, 150, 116, 255 };
static const Color OLHO  = { 230,  40,  40, 255 };
static const Color CONE  = { 240, 120,  40, 255 };
static const Color BALDE = { 150, 156, 166, 255 };

static const InfoGato infos[TOTAL_TIPOS_GATO] = {
    [GATO_COMUM] = { "Gato Zumbi",     200, 14.0f },
    [GATO_CONE]  = { "Gato de Cone",   560, 14.0f },
    [GATO_BALDE] = { "Gato de Balde", 1300, 14.0f },
};

/* gato_info: retorna um ponteiro para os dados fixos do tipo na tabela. */
const InfoGato *gato_info(TipoGato tipo)
{
    return &infos[tipo];
}

/* gato_adicionar: aloca um gato com malloc, com a vida cheia do tipo, fora da
 * tela à direita, na linha indicada, e o insere no INÍCIO da lista.
 * Retorna false se faltar memória. */
bool gato_adicionar(Gato **lista, TipoGato tipo, int linha)
{
    Gato *novo = malloc(sizeof(Gato));
    if (novo == NULL) return false;

    novo->tipo = tipo;
    novo->linha = linha;
    novo->x = X_ENTRADA;
    novo->vida = infos[tipo].vidaMaxima;
    novo->timerMordida = 0.0f;
    novo->proximo = *lista;
    *lista = novo;
    return true;
}

/* remover_no: tira da lista o nó apontado por 'ligacao' (a cabeça da lista
 * ou o campo 'proximo' do nó anterior), religando a lista ao nó seguinte, e
 * libera a memória dele. */
static void remover_no(Gato **ligacao)
{
    Gato *removido = *ligacao;
    *ligacao = removido->proximo;
    free(removido);
}

/* cachorro_na_frente: retorna o cachorro da célula onde está a boca do gato,
 * ou NULL se a célula estiver vazia ou se a boca estiver fora do campo. */
static Cachorro *cachorro_na_frente(const Gato *gato, Mapa *mapa)
{
    int linha, coluna;
    Vector2 boca = { gato->x - DISTANCIA_BOCA, mapa_centro_celula(gato->linha, 0).y };
    if (!mapa_celula_no_ponto(boca, &linha, &coluna)) return NULL;
    return mapa->grade[linha][coluna];
}

/* gato_atualizar_lista: para cada gato da lista, se houver um cachorro na
 * frente da boca, morde-o a cada INTERVALO_MORDIDA (o cachorro sem vida é
 * removido do mapa); senão, anda para a esquerda. Se algum gato passar do
 * campo, marca *chegouNaCasa. Remove (free) os gatos sem vida e retorna
 * quantos morreram neste frame. */
int gato_atualizar_lista(Gato **lista, Mapa *mapa, float dt, bool *chegouNaCasa)
{
    int mortos = 0;
    Gato **ligacao = lista;

    while (*ligacao != NULL) {
        Gato *g = *ligacao;
        Cachorro *alvo = cachorro_na_frente(g, mapa);

        if (alvo != NULL) {
            g->timerMordida -= dt;
            if (g->timerMordida <= 0.0f) {
                alvo->vida -= DANO_MORDIDA;
                g->timerMordida = INTERVALO_MORDIDA;
                if (alvo->vida <= 0) mapa_remover(mapa, alvo->linha, alvo->coluna);
            }
        } else {
            g->x -= infos[g->tipo].velocidade * dt;
        }
        if (g->x < MAPA_X - 8) *chegouNaCasa = true;

        if (g->vida <= 0) {
            remover_no(ligacao);
            mortos++;
        } else {
            ligacao = &g->proximo;
        }
    }
    return mortos;
}

/* gato_na_linha: procura o primeiro gato vivo que está na linha pedida com
 * o centro entre xMinimo e xMaximo. Retorna o ponteiro para ele, ou NULL. */
Gato *gato_na_linha(Gato *lista, int linha, float xMinimo, float xMaximo)
{
    for (Gato *g = lista; g != NULL; g = g->proximo) {
        if (g->vida > 0 && g->linha == linha && g->x >= xMinimo && g->x <= xMaximo) return g;
    }
    return NULL;
}

/* gato_desenhar_lista: desenha cada gato com formas simples (corpo, rabo,
 * cabeça virada para a casa, orelhas triangulares e olhos vermelhos),
 * balançando ao andar. O cone ou o balde aparecem enquanto o gato tiver mais
 * vida que um gato comum (como no PvZ, a proteção "cai" antes dele morrer).
 * Por cima, a barra de vida. */
void gato_desenhar_lista(const Gato *lista, float tempo)
{
    for (const Gato *g = lista; g != NULL; g = g->proximo) {
        float x = g->x;
        float y = mapa_centro_celula(g->linha, 0).y + 6 + sinf(tempo * 8.0f + x) * 1.5f;
        bool protegido = g->vida > infos[GATO_COMUM].vidaMaxima;

        DrawRectangle(x - 6, y - 6, 22, 16, PELO);
        DrawRectangle(x + 14, y - 14, 3, 12, PELO);
        DrawRectangle(x - 16, y - 16, 16, 14, PELO);
        DrawPoly((Vector2){ x - 13, y - 17 }, 3, 4, -90, PELO);
        DrawPoly((Vector2){ x - 3, y - 17 }, 3, 4, -90, PELO);
        DrawRectangle(x - 14, y - 11, 3, 3, OLHO);
        DrawRectangle(x - 7, y - 11, 3, 3, OLHO);

        if (protegido && g->tipo == GATO_CONE)  DrawPoly((Vector2){ x - 8, y - 22 }, 3, 9, -90, CONE);
        if (protegido && g->tipo == GATO_BALDE) DrawRectangle(x - 18, y - 28, 20, 13, BALDE);

        ui_barra_vida((Vector2){ x, y - 34 }, 28, (float)g->vida / infos[g->tipo].vidaMaxima);
    }
}

/* gato_liberar_lista: remove a cabeça da lista até ela ficar vazia (NULL),
 * liberando todos os nós. */
void gato_liberar_lista(Gato **lista)
{
    while (*lista != NULL) remover_no(lista);
}
