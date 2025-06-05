#include "Manipular_arquivo.h"


FILE *abrir_arquivo(const char *nome_arquivo, const char *modo)
{
    FILE *arquivo = fopen(nome_arquivo, modo);

    if (arquivo == NULL)
    {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    return arquivo;
}

void fechar_arquivo(FILE **arquivo)
{
    if (arquivo != NULL)
    {
        fclose(*arquivo);
        *arquivo = NULL;
    }
}

short int coletar_proximo_numero(FILE *arquivo, int *numero)
{
    if (arquivo == NULL || numero == NULL)
    {
        return 0; // Arquivo ou ponteiro de número inválido
    }

    if (fscanf(arquivo, "%d\n", numero) != 1)
    {
        return 0; // Não conseguiu ler um número
    }
    return 1; // Leitura bem-sucedida
}

