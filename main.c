#include <stdio.h>
#include <stdlib.h>
#include "Especificacoes.h"
#include "Teste_Rubro_negro/Teste_rubro_negro.h"

int main()
{
    
    executar_testes_rubro_negro(Diretorio_entrada_crescente, Diretorio_saida_rubro_negro, "Crescente");
    executar_testes_rubro_negro(Diretorio_entrada_decrescente, Diretorio_saida_rubro_negro, "Decrescente");
    executar_testes_rubro_negro(Diretorio_entrada_aleatorio, Diretorio_saida_rubro_negro, "Aleatório");

    return 0;
}