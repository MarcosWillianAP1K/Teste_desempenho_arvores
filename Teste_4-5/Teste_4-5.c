#include <stdio.h>
#include <stdlib.h>
#include "Teste_4-5.h"
#include "4-5.h"
#include "../Especificacoes.h"
#include "../Utilitarios/Cronometro.h"

double teste_insercao_45(Arv45 **arvore, int valor)
{
    clock_t inicio = cronometro_iniciar();

    insere_45(arvore, valor);

    clock_t tempo = cronometro_finalizar(inicio);
    return converter_para_milisegundos(tempo);
}

double teste_remocao_45(Arv45 **arvore, int valor)
{
    clock_t inicio = cronometro_iniciar();

    remover_45(arvore, valor);

    clock_t tempo = cronometro_finalizar(inicio);

    return converter_para_milisegundos(tempo);
}

double teste_consulta_45(Arv45 **arvore, int valor)
{
    clock_t inicio = cronometro_iniciar();

    buscar_45(*arvore, valor);

    clock_t tempo = cronometro_finalizar(inicio);
    return converter_para_nanosegundos(tempo);
}

void executar_testes_45(const char *diretorio_entrada, const char *diretorio_saida, const char *complementar)
{
    printf("Executando testes da árvore 4-5...\n");
    if (diretorio_entrada == NULL || diretorio_saida == NULL)
    {
        return;
    }

    Arv45 *arvore = NULL;

    double tempo_insercao = 0;
    double tempo_remocao = 0;
    double tempo_consulta1 = 0;
    double tempo_consulta2 = 0;

    FILE *arquivo = fopen(diretorio_entrada, "r");
    if (!arquivo)
    {
        printf("Erro ao abrir arquivo de entrada\n");
        return;
    }

    int valor;
    int primeiro_numero = 0, ultimo_numero = 0;

    for (int i = 0; i < QUANT_NUMEROS; i++)
    {
        if (fscanf(arquivo, "%d\n", &valor) != 1)
        {
            printf("Erro ao ler valor de entrada\n");
            break;
        }

        if (i == 0)
        {
            primeiro_numero = valor;
        }

        tempo_insercao += teste_insercao_45(&arvore, valor);
    }

    ultimo_numero = valor;
   


    // Fazer a busca do primeiro elemento inserido
    for (int j = 0; j < QUANT_DE_TESTES_CONSULTA; j++)
    {
        tempo_consulta1 += teste_consulta_45(&arvore, primeiro_numero);
    }

    // Fazer a busca do ultimo elemento inserido
    for (int j = 0; j < QUANT_DE_TESTES_CONSULTA; j++)
    {
        tempo_consulta2 += teste_consulta_45(&arvore, ultimo_numero);
    }

    rewind(arquivo);

    // Remove todos os numeros
    for (int i = 0; i < QUANT_NUMEROS; i++)
    {
        if (fscanf(arquivo, "%d", &valor) != 1)
        {
            printf("Erro ao ler valor de entrada\n");
            break;
        }

        tempo_remocao += teste_remocao_45(&arvore, valor);
    }

    libera_45(&arvore);

    fclose(arquivo);
    fflush(stdin);

    arquivo = fopen(diretorio_saida, "w");

    if (!arquivo)
    {
        printf("Erro ao abrir arquivo de saída\n");
        return;
    }

    fprintf(arquivo,"Resultados dos Testes - Árvore 4-5\n\n");
    fprintf(arquivo,"===========ESPECIFICAÇÕES===========\n\n");

    if (complementar != NULL)
    {
        fprintf(arquivo,"O de teste de inserção/remoção: %s\n", complementar);
    }
    fprintf(arquivo,"Quantidade de Numeros inseridos: %d\n", QUANT_NUMEROS);
    fprintf(arquivo,"Quantidade de Testes consulta: %d\n\n", QUANT_DE_TESTES_CONSULTA);
    fprintf(arquivo,"Primeiro numero consultado: %d\n", primeiro_numero);
    fprintf(arquivo,"Ultimo numero consultado: %d\n", ultimo_numero);

    fprintf(arquivo,"===========RESULTADOS DO TESTE===========\n\n");
    fprintf(arquivo,"Tempo medio:\n");
    fprintf(arquivo,"Tempo de Inserção: %.10lf milisegundos\n", (tempo_insercao / QUANT_NUMEROS));
    fprintf(arquivo,"Tempo de Remoção: %.10lf milisegundos\n", (tempo_remocao / QUANT_NUMEROS));
    fprintf(arquivo,"Tempo de Consulta primeiro: %.10lf nanosegundos\n", (tempo_consulta1 / QUANT_DE_TESTES_CONSULTA));
    fprintf(arquivo,"Tempo de Consulta ultimo: %.10lf nanosegundos\n\n", (tempo_consulta2 / QUANT_DE_TESTES_CONSULTA));

    fprintf(arquivo,"Tempo total:\n");
    fprintf(arquivo,"Tempo total de Inserção: %.10lf milisegundos\n", tempo_insercao);
    fprintf(arquivo,"Tempo total de Remoção: %.10lf milisegundos\n", tempo_remocao);
    fprintf(arquivo,"Tempo total de Consulta primeiro: %.10lf nanosegundos\n", tempo_consulta1);
    fprintf(arquivo,"Tempo total de Consulta ultimo: %.10lf nanosegundos\n\n", tempo_consulta2);
    fprintf(arquivo,"=========================================");

    fclose(arquivo);
}