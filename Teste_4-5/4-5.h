#pragma once

// Estrutura do No da Arvore 4-5
typedef struct Arv45
{
    int info1, info2, info3, info4;
    int nInfo; // 0 (underflow temporario), 1, 2, 3 ou 4
    struct Arv45 *esq;
    struct Arv45 *f_cen1;
    struct Arv45 *f_cen2;
    struct Arv45 *f_cen3;
    struct Arv45 *dir;
} Arv45;



int *buscar_45(Arv45 *raiz, int info);

Arv45 *cria_no_45(int info, Arv45 *F_esq, Arv45 *F_f_cen1);

int insere_45(Arv45 **raiz, int valor);

void imprime_45_em_ordem(Arv45 *raiz);

void imprime_arvore_visual_45(Arv45 *raiz, char *prefixo, int eh_ultimo, int eh_raiz);

int remover_45(Arv45 **raiz, int valor);

void libera_45(Arv45 **raiz);