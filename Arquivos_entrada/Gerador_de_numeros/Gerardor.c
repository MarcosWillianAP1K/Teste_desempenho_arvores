#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define QUANT_NUMEROS 100

#define Arquivo_cresente "../../numeros.txt"
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

    srand(time(NULL)); // Semente para números aleatórios

    for (int i = 0; i < QUANT_NUMEROS; i++)
    {

        int numero = rand() % QUANT_NUMEROS + 1; // Gera número entre 1 e QUANT_NUMEROS
        
        while (verificar_se_numero_ja_existe(arquivo, numero))
        {
            numero = rand() % QUANT_NUMEROS + 1; // Gera novo número se já existir
        }
        
        fprintf(arquivo, "%d\n", numero);
    }

    fclose(arquivo);
}

int main()
{


    return 0;
}
