#include <stdio.h>
#include <stdlib.h>
#include "Teste_rubro_negro.h"
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
    printf("Executando testes da árvore rubro-negra...\n");
    if (diretorio_entrada == NULL || diretorio_saida == NULL)
    {
        return;
    }

    ArvLLRB arvore = NULL;

    long double tempo_insercao = 0;
    long double tempo_remocao = 0;
    long double tempo_consulta1 = 0;
    long double tempo_consulta2 = 0;
    printf("bah\n");
    FILE *arquivo = abrir_arquivo(diretorio_entrada, "r");
    printf("bah1\n");
    int valor;
    int primeiro_numero, ultimo_numero;

    for (int i = 0; i < QUANT_NUMEROS; i++)
    {
        printf("bah2\n");

        if (!coletar_proximo_numero(arquivo, &valor))
        {
            printf("Erro ao ler valor de entrada\n");
            break;
        }

        if (i == 0)
        {
            primeiro_numero = valor;
        }
        

        tempo_insercao += teste_insercao_rubro_negro(&arvore, valor);

        printf("bah3\n");
    }

    ultimo_numero = valor;

    // Fazer a busca do primeiro elemento inserido
    for (int j = 0; j < QUANT_DE_TESTES_CONSULTA; j++)
    {
        printf("bah4\n");
        tempo_consulta1 += teste_consulta_rubro_negro(&arvore, primeiro_numero);
    }

    // Fazer a busca do ultimo elemento inserido
    for (int j = 0; j < QUANT_DE_TESTES_CONSULTA; j++)
    {
        printf("bah4\n");
        tempo_consulta2 += teste_consulta_rubro_negro(&arvore, ultimo_numero);
    }

    rewind(arquivo);

    printf("bah4.5\n");
    // Remove todos os numeros
    for (int i = 0; i < QUANT_NUMEROS; i++)
    {
        if (!coletar_proximo_numero(arquivo, &valor))
        {
            printf("Erro ao ler valor de entrada\n");
            break;
        }

        tempo_insercao += teste_remocao_rubro_negro(&arvore, valor);
    }

    printf("bah5\n");

    fechar_arquivo(&arquivo);

    printf("bah6\n");

    arquivo = freopen(diretorio_saida, "w", stdout);

    if (!arquivo)
    {
        printf("Erro ao abrir arquivo de saída\n");
        return;
    }

    printf("Resultados dos Testes - Árvore Rubro-Negra\n\n");
    printf("===========ESPECIFICAÇÕES===========\n\n");

    if (complementar != NULL)
    {
        printf("O de teste de inserção/remoção: %s\n", complementar);
    }
    printf("Quantidade de Numeros inseridos: %d\n", QUANT_NUMEROS);
    printf("Quantidade de Testes consulta: %d\n\n", QUANT_DE_TESTES_CONSULTA);
    printf("Primeiro numero consultado: %d\n", primeiro_numero);
    printf("Ultimo numero consultado: %d\n", ultimo_numero);

    printf("===========RESULTADOS DO TESTE===========\n\n");
    printf("Tempo medio:\n");
    printf("Tempo de Inserção: %.10lf milisegundos\n", (double)(tempo_insercao / QUANT_NUMEROS));
    printf("Tempo de Remoção: %.10lf milisegundos\n", (double)(tempo_remocao / QUANT_NUMEROS));
    printf("Tempo de Consulta primeiro: %.10lf nanosegundos\n", (double)(tempo_consulta1 / QUANT_DE_TESTES_CONSULTA));
    printf("Tempo de Consulta ultimo: %.10lf nanosegundos\n\n", (double)(tempo_consulta2 / QUANT_DE_TESTES_CONSULTA));

    printf("Tempo total:\n");
    printf("Tempo total de Inserção: %.10lf milisegundos\n", (double)tempo_insercao);
    printf("Tempo total de Remoção: %.10lf milisegundos\n", (double)tempo_remocao);
    printf("Tempo total de Consulta primeiro: %.10lf nanosegundos\n", (double)tempo_consulta1);
    printf("Tempo total de Consulta ultimo: %.10lf nanosegundos\n\n", (double)tempo_consulta2);
    printf("=========================================");

    fechar_arquivo(&arquivo);
}