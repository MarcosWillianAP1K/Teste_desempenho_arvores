// http://www.teachsolaisgames.com/articles/balanced_left_leaning.html

#include <stdio.h>
#include <stdlib.h>
#include "Rubro_negro.h"



//========================ALOCAÇÃO e LIBERAÇÃO========================

RUBRO_NEGRO *alocar_no_rubro_negro()
{
    RUBRO_NEGRO *no = (RUBRO_NEGRO *)malloc(sizeof(RUBRO_NEGRO));

    if (no == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória para nó da árvore rubro-negra\n");
        exit(EXIT_FAILURE);
    }

    no->cor = VERMELHO;
    no->esquerda = NULL;
    no->direita = NULL;
    
    return no;
}

void liberar_no_rubro_negro(RUBRO_NEGRO **no)
{
    if (no != NULL && *no != NULL)
    {
        free(*no);
        *no = NULL;
    }
}

void liberar_rubro_negro(RUBRO_NEGRO **raiz)
{
    if (raiz != NULL && *raiz != NULL)
    {
        liberar_rubro_negro(&(*raiz)->esquerda);
        liberar_rubro_negro(&(*raiz)->direita);

        liberar_no_rubro_negro(raiz);
    }
}

void liberar_rubro_negro_void(void **raiz)
{
    RUBRO_NEGRO *arvore = *raiz;
    liberar_rubro_negro(&arvore);
    *raiz = arvore;
}

//========================BUSCA========================

RUBRO_NEGRO *buscar_rubro_negro(RUBRO_NEGRO *raiz, int aux)
{
    RUBRO_NEGRO *retorno = NULL;

    if (raiz != NULL)
    {
        if (raiz->info == aux)
        {
            retorno = raiz;
        }
        else
        {
            retorno = buscar_rubro_negro(raiz->esquerda, aux);

            if (retorno == NULL)
                retorno = buscar_rubro_negro(raiz->direita, aux);
        }
    }

    return retorno;
}


//========================ROTACIONAR========================

void de_ladinho_para_direita(RUBRO_NEGRO **raiz)
{
    if (raiz != NULL && *raiz != NULL)
    {
        RUBRO_NEGRO *aux = (*raiz)->esquerda;
        aux->cor = (*raiz)->cor;
        (*raiz)->cor = VERMELHO;
        (*raiz)->esquerda = aux->direita;
        aux->direita = *raiz;
        *raiz = aux;
    }
}

void de_ladinho_para_esquerda(RUBRO_NEGRO **raiz)
{
    if (raiz != NULL && *raiz != NULL)
    {
        RUBRO_NEGRO *aux = (*raiz)->direita;
        aux->cor = (*raiz)->cor;
        (*raiz)->cor = VERMELHO;
        (*raiz)->direita = aux->esquerda;
        aux->esquerda = *raiz;
        *raiz = aux;
    }
}


//========================COR========================

COR cor(RUBRO_NEGRO *no)
{
    COR retorno = PRETO;

    if (no != NULL)
    {
        retorno = no->cor;
    }

    return retorno;
}

void trocar_cor(RUBRO_NEGRO *no)
{
    if (no != NULL)
    {
        if (no->cor == VERMELHO)
        {
            no->cor = PRETO;
        }
        else
        {
            no->cor = VERMELHO;
        }

        if (no->esquerda != NULL)
        {
            if (cor(no->esquerda) == VERMELHO)
            {
                no->esquerda->cor = PRETO;
            }
            else
            {
                no->esquerda->cor = VERMELHO;
            }
        }

        if (no->direita != NULL)
        {
            if (cor(no->direita) == VERMELHO)
            {
                no->direita->cor = PRETO;
            }
            else
            {
                no->direita->cor = VERMELHO;
            }
        }
    }
}


//========================BALANCEAMENTO========================

void balancear(RUBRO_NEGRO **raiz)
{

    if (cor((*raiz)->esquerda) == PRETO && cor((*raiz)->direita) == VERMELHO)
    {
        de_ladinho_para_esquerda(raiz);
    }

    if (cor((*raiz)->esquerda) == VERMELHO && cor((*raiz)->esquerda->esquerda))
    {
        de_ladinho_para_direita(raiz);
    }

    if (cor((*raiz)->esquerda) == VERMELHO && cor((*raiz)->direita) == VERMELHO)
    {
        trocar_cor(*raiz);
    }
}


//========================INSERÇÃO========================

RUBRO_NEGRO *inserir_rubro_negro_recursivo(RUBRO_NEGRO **raiz, int info)
{
    RUBRO_NEGRO *retorno = NULL;

    if (*raiz == NULL)
    {
        *raiz = alocar_no_rubro_negro();
        (*raiz)->info = info;
        retorno = *raiz;
    }
    else
    {
        if ((*raiz)->info > info)
        {
            retorno = inserir_rubro_negro_recursivo(&(*raiz)->esquerda, info);
        }
        else if ((*raiz)->info < info)
        {
            retorno = inserir_rubro_negro_recursivo(&(*raiz)->direita, info);
        }

        if (retorno != NULL)
        {
            balancear(raiz);
        }
    }

    return retorno;
}

RUBRO_NEGRO *inserir_rubro_negro(RUBRO_NEGRO **raiz, int info)
{
    RUBRO_NEGRO *retorno = NULL;

    if (raiz != NULL)
    {
        retorno = inserir_rubro_negro_recursivo(raiz, info);
        (*raiz)->cor = PRETO;
    }

    return retorno;
}

RUBRO_NEGRO *inserir_rubro_negro_void(void **raiz, int info)
{
    RUBRO_NEGRO *retorno = NULL;
    RUBRO_NEGRO *arvore = *raiz;

    retorno = inserir_rubro_negro(&arvore, info);
    *raiz = arvore;

    return retorno;
}

//========================REMOÇÃO========================

void move2EsquerdaVERMELHO(RUBRO_NEGRO **no)
{
    trocar_cor(*no);
    if (cor((*no)->direita->esquerda) == VERMELHO)
    {
        de_ladinho_para_direita(&(*no)->direita);
        de_ladinho_para_esquerda(&(*no));
        trocar_cor(*no);
    }
}

// Função para mover um nó vermelho para a direita
void move2DireitaVERMELHO(RUBRO_NEGRO **no)
{
    trocar_cor(*no);
    if (cor((*no)->esquerda->esquerda) == VERMELHO)
    {
        de_ladinho_para_direita(&(*no));
        trocar_cor(*no);
    }
}

// Função para procurar o menor elemento
RUBRO_NEGRO *procura_Menor(RUBRO_NEGRO *atual)
{
    while (atual->esquerda != NULL)
    {
        atual = atual->esquerda;
    }

    return atual;
}

// Função para remover o menor elemento
//TROCAR O RETORNO PARA AQUI INVES DE DAR FREE
RUBRO_NEGRO *remover_no_Menor(RUBRO_NEGRO **raiz)
{
    RUBRO_NEGRO *retorno = NULL;

    if ((*raiz)->esquerda == NULL)
    {
        retorno = *raiz;
        *raiz = NULL;
        return retorno;
    }
    if (cor((*raiz)->esquerda) == PRETO && cor((*raiz)->esquerda->esquerda) == PRETO)
        move2EsquerdaVERMELHO(raiz);

    remover_no_Menor(&(*raiz)->esquerda);
    balancear(raiz);
}

RUBRO_NEGRO *remover_rubro_negro_recursivo(RUBRO_NEGRO **raiz, int aux)
{
    RUBRO_NEGRO *retorno = NULL;

    if (aux < (*raiz)->info)
    {
        if (cor((*raiz)->esquerda) == PRETO && cor((*raiz)->esquerda->esquerda) == PRETO)
            move2EsquerdaVERMELHO(raiz);

        retorno = remover_rubro_negro_recursivo(&(*raiz)->esquerda, aux);
    }
    else
    {
        if (cor((*raiz)->esquerda) == VERMELHO)
            de_ladinho_para_direita(raiz);

        if (aux == (*raiz)->info && (*raiz)->direita == NULL)
        {
            retorno = *raiz;
            *raiz = NULL;

            return retorno;
        }

        if (cor((*raiz)->direita) == PRETO && cor((*raiz)->direita->esquerda) == PRETO)
            move2DireitaVERMELHO(raiz);

        if (aux  == (*raiz)->info)
        {
            RUBRO_NEGRO *aux = procura_Menor((*raiz)->direita);

            // Precisa criar um copia do no para retornar.
            // retorno = alocar_no_rubro_negro();
            // retorno->info = (*raiz)->info;
            // retorno->cor = (*raiz)->cor;

            // remover_no_Menor(&(*raiz)->direita);

            int aux_dado = (*raiz)->info;
            (*raiz)->info = aux->info;
            aux->info = aux_dado;

            retorno = remover_no_Menor(&(*raiz)->direita);
        }
        else
        {
            retorno = remover_rubro_negro_recursivo(&(*raiz)->direita, aux);
        }
    }

    balancear(raiz);

    return retorno;
}

// Retorna o dado do no removido, precisando liberar fora da função
RUBRO_NEGRO *remover_rubro_negro(RUBRO_NEGRO **raiz, int aux)
{
    RUBRO_NEGRO *retorno = NULL;

    if (buscar_rubro_negro(*raiz, aux) != NULL)
    {
        retorno = remover_rubro_negro_recursivo(raiz, aux);

        if (*raiz != NULL)
            (*raiz)->cor = PRETO;
    }

    return retorno;
}


RUBRO_NEGRO *remover_rubro_negro_void(void **raiz, int aux)
{
    RUBRO_NEGRO *retorno = NULL;

    RUBRO_NEGRO *arvore = *raiz;
    retorno = remover_rubro_negro(&arvore, aux);
    *raiz = arvore;

    return retorno;
}


//========================IMPRIMIR========================

short int imprimir_rubro_negro(RUBRO_NEGRO *raiz)
{
    short int retorno = 0;

    if (raiz != NULL)
    {
        retorno = imprimir_rubro_negro(raiz->esquerda);

        printf("%d ", raiz->info);

        retorno = imprimir_rubro_negro(raiz->direita);

        retorno = 1;
    }
    

    return retorno;
}

short int imprimir_filtro_rubro_negro(RUBRO_NEGRO *raiz, int aux)
{
    short int retorno = 0;

    if (raiz != NULL)
    {
        retorno |= imprimir_filtro_rubro_negro(raiz->esquerda, aux);

        if (raiz->info == aux)
        {
            printf("%d ", raiz->info);
            retorno = 1;
        }

        retorno |= imprimir_filtro_rubro_negro(raiz->direita, aux);
    }



    return retorno;
}
