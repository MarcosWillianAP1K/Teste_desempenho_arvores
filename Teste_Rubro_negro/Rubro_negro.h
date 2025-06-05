
#pragma once

typedef struct NO *ArvLLRB;

// ABB RUBRO NEGRO 2-3
ArvLLRB* cria_ArvLLRB();
void libera_ArvLLRB(ArvLLRB* raiz);
int insere_ArvLLRB(ArvLLRB* raiz,int valor);
int remove_ArvLLRB(ArvLLRB *raiz, int valor);
int consulta_ArvLLRB(ArvLLRB *raiz, int valor);