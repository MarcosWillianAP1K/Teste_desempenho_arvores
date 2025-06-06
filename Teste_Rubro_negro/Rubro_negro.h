
#pragma once


typedef enum COR
{
    PRETO,
    VERMELHO
}COR;

typedef struct RUBRO_NEGRO
{
    int info;
    struct RUBRO_NEGRO *esquerda;
    struct RUBRO_NEGRO *direita;
    COR cor;

}RUBRO_NEGRO;

RUBRO_NEGRO *alocar_no_rubro_negro();

void liberar_no_rubro_negro(RUBRO_NEGRO **no);

void liberar_rubro_negro(RUBRO_NEGRO **raiz);

void liberar_rubro_negro_void(void **raiz);

RUBRO_NEGRO *buscar_rubro_negro(RUBRO_NEGRO *raiz, int aux);

COR cor(RUBRO_NEGRO *no);

RUBRO_NEGRO *inserir_rubro_negro(RUBRO_NEGRO **raiz, int info);

RUBRO_NEGRO *inserir_rubro_negro_void(void **raiz, int info);

RUBRO_NEGRO *remover_rubro_negro(RUBRO_NEGRO **raiz, int aux);

RUBRO_NEGRO *remover_rubro_negro_void(void **raiz, int aux);

short int imprimir_rubro_negro(RUBRO_NEGRO *raiz);

short int imprimir_filtro_rubro_negro(RUBRO_NEGRO *raiz, int aux);