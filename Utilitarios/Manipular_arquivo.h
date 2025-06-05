#include <stdio.h>
#include <stdlib.h>


#pragma once

FILE *abrir_arquivo(const char *nome_arquivo, const char *modo);

void fechar_arquivo(FILE **arquivo);

short int coletar_proximo_numero(FILE *arquivo, int *numero);