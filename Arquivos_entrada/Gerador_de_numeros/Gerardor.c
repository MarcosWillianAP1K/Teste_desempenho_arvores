#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define QUANT_NUMEROS 10000

#define Arquivo_cresente "../../numeros_cresente.txt"
#define Arquivo_decrescente "../../numeros_decrescente.txt"
#define Arquivo_aleatorio "../../numeros_aleatorios.txt"

void gerarNumerosCrescente()
{
    FILE *arquivo = fopen(Arquivo_cresente, "w");
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

short int verificar_se_numero_ja_existe(int vetor[], int numero)
{
    for (int i = 0; i < QUANT_NUMEROS; i++)
    {
        if (vetor[i] == numero)
        {
            return 1; // Número já existe
        }
    }
    return 0; // Número não existe
}

void gerarNumerosAleatorios()
{
    FILE *arquivo = fopen(Arquivo_aleatorio, "w");
    if (arquivo == NULL)
    {
        perror("Erro ao abrir o arquivo para escrita");
        return;
    }

    int vetor[QUANT_NUMEROS] = {0}; // Vetor para verificar números já existentes

    srand(time(NULL)); // Semente para números aleatórios

    for (int i = 0; i < QUANT_NUMEROS; i++)
    {

        int numero = rand() % QUANT_NUMEROS + 1; // Gera número entre 1 e QUANT_NUMEROS
        
        while (verificar_se_numero_ja_existe(vetor, numero))
        {
            numero = rand() % QUANT_NUMEROS + 1; // Gera novo número se já existir
        }
        
        fprintf(arquivo, "%d\n", numero);
    }

    fclose(arquivo);
}

void verificar_arquivos_foram_gerados()
{
    FILE *arquivoCrescente = fopen(Arquivo_cresente, "r");
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
