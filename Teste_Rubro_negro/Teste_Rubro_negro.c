#include <stdio.h>
#include <stdlib.h>
#include "Rubro_negro.h"
#include "../Especificacoes.h"
#include "../Utilitarios/Cronometro.h"

double teste_insercao_rubro_negro(ArvLLRB *arvore)
{
    
    

}

double teste_remocao_rubro_negro(ArvLLRB *arvore)
{
    // Implementar teste de remoção para árvore rubro-negra
}

double teste_consulta_rubro_negro(ArvLLRB *arvore)
{
    // Implementar teste de consulta para árvore rubro-negra
}

void executar_testes_rubro_negro()
{
    ArvLLRB  arvore = NULL;

    for (int i = 0; i < QUANT_DE_TESTES_INSERCAO_REMOCAO; i++)
    {
        teste_insercao_rubro_negro(&arvore);

        for (int j = 0; j < QUANT_DE_TESTES_CONSULTA && i == 0; j++)
        {
            teste_consulta_rubro_negro(&arvore);
        }

        teste_remocao_rubro_negro(&arvore);
    }

    libera_ArvLLRB(&arvore);
    arvore = NULL;
}