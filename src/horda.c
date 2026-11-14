#include "horda.h"
#include "config.h"
#include "ui.h"

#include <stdlib.h>

#define PRIMEIRA_HORDA      20.0f
#define PAUSA_ENTRE_HORDAS   6.0f
#define ESPERA_MAXIMA       30.0f
#define DURACAO_ANUNCIO      2.5f

static const int composicao[TOTAL_FASES][HORDAS_POR_FASE][TOTAL_TIPOS_GATO] = {
    {
        { 3, 0, 0, 0, 0, 0, 0 },
        { 4, 1, 0, 0, 0, 0, 0 },
        { 5, 2, 0, 0, 0, 0, 0 },
        { 7, 3, 0, 0, 0, 0, 0 },
    },
    {
        { 3, 1, 0, 1, 0, 0, 0 },
        { 4, 2, 1, 1, 0, 0, 0 },
        { 5, 2, 1, 2, 0, 0, 0 },
        { 7, 3, 2, 3, 0, 0, 0 },
    },
    {
        { 3, 2, 0, 1, 1, 0, 0 },
        { 4, 2, 1, 1, 1, 1, 0 },
        { 5, 3, 1, 2, 2, 2, 0 },
        { 7, 3, 2, 3, 2, 3, 0 },
    },
    {
        { 4, 2, 1, 1, 1, 1, 0 },
        { 5, 3, 1, 2, 2, 2, 0 },
        { 5, 3, 2, 2, 2, 2, 1 },
        { 8, 4, 3, 3, 3, 3, 2 },
    },
    {
        { 5, 3, 2, 2, 2, 2, 1 },
        { 6, 3, 2, 3, 2, 3, 1 },
        { 6, 4, 3, 3, 3, 3, 2 },
        { 8, 4, 4, 4, 3, 4, 3 },
    },
};

static const float intervalo[HORDAS_POR_FASE] = { 7.0f, 6.0f, 5.0f, 3.0f };

void horda_iniciar(Horda *horda)
{
    *horda = (Horda){ 0 };
    horda->esperando = true;
    horda->timer = PRIMEIRA_HORDA;
}

static void montar_fila(Horda *horda, int fase)
{
    horda->total = 0;
    horda->proximo = 0;
    for (int tipo = 0; tipo < TOTAL_TIPOS_GATO; tipo++) {
        for (int k = 0; k < composicao[fase - 1][horda->atual][tipo] && horda->total < MAX_GATOS_HORDA; k++) {
            horda->fila[horda->total++] = (TipoGato)tipo;
        }
    }
    for (int i = horda->total - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        TipoGato troca = horda->fila[i];
        horda->fila[i] = horda->fila[j];
        horda->fila[j] = troca;
    }
}

void horda_atualizar(Horda *horda, int fase, Gato **gatos, float dt)
{
    if (horda->atual >= HORDAS_POR_FASE) return;
    horda->timer -= dt;
    if (horda->timerAnuncio > 0.0f) horda->timerAnuncio -= dt;

    if (horda->esperando) {
        if (horda->timer > 0.0f) return;
        montar_fila(horda, fase);
        horda->esperando = false;
        horda->timer = 0.0f;
        horda->timerAnuncio = DURACAO_ANUNCIO;
        return;
    }

    if (horda->proximo < horda->total) {
        if (horda->timer > 0.0f) return;
        gato_adicionar(gatos, horda->fila[horda->proximo++], rand() % LINHAS_MAPA);
        horda->timer = (horda->proximo < horda->total) ? intervalo[horda->atual] : ESPERA_MAXIMA;
        return;
    }

    bool ultima = (horda->atual == HORDAS_POR_FASE - 1);
    if (*gatos != NULL && (ultima || horda->timer > 0.0f)) return;
    horda->atual++;
    horda->esperando = true;
    horda->timer = PAUSA_ENTRE_HORDAS;
}

bool horda_terminou(const Horda *horda)
{
    return horda->atual >= HORDAS_POR_FASE;
}

float horda_progresso(const Horda *horda)
{
    if (horda->atual >= HORDAS_POR_FASE) return 1.0f;
    float parcial = (!horda->esperando && horda->total > 0) ? (float)horda->proximo / horda->total : 0.0f;
    return (horda->atual + parcial) / HORDAS_POR_FASE;
}

void horda_desenhar(const Horda *horda)
{
    Rectangle barra = { 456, 353, 170, 5 };
    DrawRectangleRec(barra, COR_SOMBRA);
    DrawRectangle(barra.x, barra.y, barra.width * horda_progresso(horda), barra.height, COR_ALERTA);
    for (int i = 1; i <= HORDAS_POR_FASE; i++) {
        DrawRectangle(barra.x + barra.width * i / HORDAS_POR_FASE - 2, barra.y - 1, 2, barra.height + 2, COR_TEXTO);
    }

    if (horda->timerAnuncio <= 0.0f) return;
    bool final = (horda->atual == HORDAS_POR_FASE - 1);
    ui_texto_centro(final ? "HORDA FINAL!" : TextFormat("HORDA %d", horda->atual + 1), LARGURA_VIRTUAL / 2, 200, FONTE_GRANDE, COR_ALERTA);
}
