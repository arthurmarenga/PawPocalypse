/* projetil.c - Lista encadeada de latidos. */

#include "projetil.h"
#include "config.h"
#include "ui.h"

#include <stdlib.h>

#define RAIO_ACERTO 12.0f

/* projetil_adicionar: aloca um latido com malloc na linha e posição dadas e
 * o insere no INÍCIO da lista. Retorna false se faltar memória. */
bool projetil_adicionar(Projetil **lista, int linha, float x, int dano)
{
    Projetil *novo = malloc(sizeof(Projetil));
    if (novo == NULL) return false;

    novo->linha = linha;
    novo->x = x;
    novo->dano = dano;
    novo->proximo = *lista;
    *lista = novo;
    return true;
}

/* remover_no: tira da lista o nó apontado por 'ligacao' (a cabeça da lista
 * ou o campo 'proximo' do nó anterior), religando a lista ao nó seguinte, e
 * libera a memória dele. */
static void remover_no(Projetil **ligacao)
{
    Projetil *removido = *ligacao;
    *ligacao = removido->proximo;
    free(removido);
}

/* projetil_atualizar_lista: move cada latido para a direita. Se ele encostar
 * num gato da mesma linha, tira 'dano' da vida do gato e é removido; também
 * é removido ao sair da tela. */
void projetil_atualizar_lista(Projetil **lista, Gato *gatos, float dt)
{
    Projetil **ligacao = lista;
    while (*ligacao != NULL) {
        Projetil *p = *ligacao;
        p->x += VELOCIDADE_LATIDO * dt;

        Gato *alvo = gato_na_linha(gatos, p->linha, p->x - RAIO_ACERTO, p->x + RAIO_ACERTO);
        if (alvo != NULL) alvo->vida -= p->dano;

        if (alvo != NULL || p->x > LARGURA_VIRTUAL) remover_no(ligacao);
        else                                        ligacao = &p->proximo;
    }
}

/* projetil_desenhar_lista: desenha cada latido como dois arcos laranja,
 * parecendo uma onda de som indo para a direita. */
void projetil_desenhar_lista(const Projetil *lista)
{
    for (const Projetil *p = lista; p != NULL; p = p->proximo) {
        Vector2 centro = { p->x, mapa_centro_celula(p->linha, 0).y - 4 };
        DrawRing(centro, 4, 7, -55, 55, 10, COR_PETISCO);
        DrawRing(centro, 10, 13, -55, 55, 10, Fade(COR_PETISCO, 0.6f));
    }
}

/* projetil_liberar_lista: remove a cabeça da lista até ela ficar vazia
 * (NULL), liberando todos os nós. */
void projetil_liberar_lista(Projetil **lista)
{
    while (*lista != NULL) remover_no(lista);
}
