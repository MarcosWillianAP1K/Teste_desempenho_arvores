#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../../Especificacoes.h"



#define Arquivo_crescente "../../numeros_crescente.txt"
#define Arquivo_decrescente "../../numeros_decrescente.txt"
#define Arquivo_aleatorio "../../numeros_aleatorios.txt"

void gerarNumerosCrescente()
{
    FILE *arquivo = fopen(Arquivo_crescente, "w");
    if (arquivo == NULL)
    {
        perror("Erro ao abrir o arquivo para escrita");
        return;
    }

    for (int i = 1; i <= QUANT_NUMEROS; i++)
    {
        fprintf(arquivo, "%d\n", i);
    }

    fclose(arquivo);
}

void gerarNumerosDecrescente()
{
    FILE *arquivo = fopen(Arquivo_decrescente, "w");
    if (arquivo == NULL)
    {
        perror("Erro ao abrir o arquivo para escrita");
        return;
    }

    for (int i = QUANT_NUMEROS; i >= 1; i--)
    {
        fprintf(arquivo, "%d\n", i);
    }

    fclose(arquivo);
}


void gerarNumerosAleatorios()

{
    FILE *arquivo = fopen(Arquivo_aleatorio, "w");
    if (arquivo == NULL)
    {
        perror("Erro ao abrir o arquivo para escrita");
        return;
    }

    srand(time(NULL)); // Semente para números aleatórios
    
    // Usar array para rastrear números já gerados
    int *numeros_usados = (int *)calloc(QUANT_NUMEROS + 1, sizeof(int));
    if (numeros_usados == NULL) {
        perror("Erro ao alocar memória");
        fclose(arquivo);
        return;
    }
    
    int contador = 0;
    while (contador < QUANT_NUMEROS)
    {
        int numero = rand() % QUANT_NUMEROS + 1; // Gera número entre 1 e QUANT_NUMEROS
        
        // Verifica se o número já foi usado
        if (numeros_usados[numero] == 0) {
            numeros_usados[numero] = 1; // Marca como usado
            fprintf(arquivo, "%d\n", numero);
            contador++;
        }
    }
    
    free(numeros_usados);
    fclose(arquivo);
}

void verificar_arquivos_foram_gerados()
{
    FILE *arquivoCrescente = fopen(Arquivo_crescente, "r");
    FILE *arquivoDecrescente = fopen(Arquivo_decrescente, "r");
    FILE *arquivoAleatorio = fopen(Arquivo_aleatorio, "r");

    if (arquivoCrescente == NULL)
    {
        printf("Erro ao abrir os arquivos gerados.\n\n");
    }

    if (arquivoDecrescente == NULL)
    {
        printf("Erro ao abrir os arquivos gerados.\n\n");
    }

    if (arquivoAleatorio == NULL)
    {
        printf("Erro ao abrir os arquivos gerados.\n\n");
    }

    fclose(arquivoCrescente);
    fclose(arquivoDecrescente);
    fclose(arquivoAleatorio);

    if (arquivoCrescente != NULL && arquivoDecrescente != NULL && arquivoAleatorio != NULL)
    {
        printf("Todos os arquivos foram gerados com sucesso!\n\n");
    }
    else
    {
        printf("Alguns arquivos nao foram gerados corretamente.\n\n");
    }
}

int main()
{

    printf("Gerando numeros em ordem crescente...\n");
    gerarNumerosCrescente();
    printf("Sucesso! Numeros em ordem crescente gerados.\n\n");

    printf("Gerando numeros em ordem decrescente...\n");
    gerarNumerosDecrescente();
    printf("Sucesso! Numeros em ordem decrescente gerados.\n\n");

    printf("Gerando numeros aleatorios...\n");
    gerarNumerosAleatorios();
    printf("Sucesso! Numeros aleatorios gerados.\n\n");

    verificar_arquivos_foram_gerados();

    return 0;
}
