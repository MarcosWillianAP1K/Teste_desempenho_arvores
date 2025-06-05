#include <stdio.h>
#include <stdlib.h>
#include "Rubro_negro.h"
#include "../Especificacoes.h"
#include "../Utilitarios/Cronometro.h"
#include "../Utilitarios/Manipular_arquivo.h"

long double teste_insercao_rubro_negro(ArvLLRB *arvore, int valor)
{
    clock_t inicio = cronometro_iniciar();

    if (!insere_ArvLLRB(arvore, valor))
        printf("Erro ao inserir valor %d na árvore rubro-negra\n", valor);

    clock_t tempo = cronometro_finalizar(inicio);
    return converter_para_milisegundos(tempo);
}

long double teste_remocao_rubro_negro(ArvLLRB *arvore, int valor)
{
    clock_t inicio = cronometro_iniciar();

    if (!remove_ArvLLRB(arvore, valor))
        printf("Erro ao remover valor %d da árvore rubro-negra\n", valor);

    clock_t tempo = cronometro_finalizar(inicio);
    return converter_para_milisegundos(tempo);
}

long double teste_consulta_rubro_negro(ArvLLRB *arvore, int valor)
{
    clock_t inicio = cronometro_iniciar();

    if (!consulta_ArvLLRB(arvore, valor))
        printf("Erro ao consultar valor %d na árvore rubro-negra\n", valor);


    clock_t tempo = cronometro_finalizar(inicio);
    return converter_para_nanosegundos(tempo);
}

void executar_testes_rubro_negro(const char *diretorio_entrada, const char *diretorio_saida, const char *complementar)
{
    if (diretorio_entrada != NULL || diretorio_saida != NULL)
    {
        return;
    }
    

    ArvLLRB arvore = NULL;

    long double tempo_insercao = 0;
    long double tempo_remocao = 0;
    long double tempo_consulta = 0;

    FILE *arquivo = abrir_arquivo(diretorio_entrada, "r");
    int valor;

    for (int i = 0; i < QUANT_DE_TESTES_INSERCAO_REMOCAO; i++)
    {
        if (!coletar_proximo_numero(arquivo, &valor))
        {
            printf("Erro ao ler valor de entrada\n");
            break;
        }

        tempo_insercao += teste_insercao_rubro_negro(&arvore, valor);

        for (int j = 0; j < QUANT_DE_TESTES_CONSULTA && i == 0; j++)
        {
            tempo_consulta += teste_consulta_rubro_negro(&arvore, valor);
        }

        tempo_remocao += teste_remocao_rubro_negro(&arvore, valor);

        libera_ArvLLRB(&arvore);
        arvore = NULL;
    }

    fechar_arquivo(&arquivo);





    arquivo = freopen(diretorio_saida, "w", stdout);


    if (!arquivo) {
        printf("Erro ao abrir arquivo de saída\n");
        return;
    }

    printf("Resultados dos Testes - Árvore Rubro-Negra\n\n");
    printf("===========ESPECIFICAÇÕES===========\n\n");

    if (complementar != NULL) {
        printf("O de teste de inserção/remoção: %s\n", complementar);
    }
    printf("Quantidade de Testes inserção/remoção: %d\n", QUANT_DE_TESTES_INSERCAO_REMOCAO);
    printf("Quantidade de Testes consulta: %d\n\n", QUANT_DE_TESTES_CONSULTA);

    printf("===========RESULTADOS DO TESTE===========\n\n");
    printf("Tempo medio:\n");
    printf("Tempo de Inserção: %.10lf milisegundos\n", (double)(tempo_insercao / QUANT_DE_TESTES_INSERCAO_REMOCAO));
    printf("Tempo de Remoção: %.10lf milisegundos\n", (double)(tempo_remocao / QUANT_DE_TESTES_INSERCAO_REMOCAO));
    printf("Tempo de Consulta: %.10lf nanosegundos\n\n", (double)(tempo_consulta / QUANT_DE_TESTES_CONSULTA));

    printf("Tempo total:\n");
    printf("Tempo total de Inserção: %.10lf milisegundos\n", (double)tempo_insercao);
    printf("Tempo total de Remoção: %.10lf milisegundos\n", (double)tempo_remocao);
    printf("Tempo total de Consulta: %.10lf nanosegundos\n", (double)tempo_consulta);
    printf("=========================================\n");

    fechar_arquivo(&arquivo);
}