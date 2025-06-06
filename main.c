#include <stdio.h>
#include <stdlib.h>
#include "Especificacoes.h"
#include "Teste_Rubro_negro/Teste_rubro_negro.h"
#include"Teste_2-3/Teste_2-3.h"

int main()
{
    
    // executar_testes_rubro_negro(Diretorio_entrada_crescente, Diretorio_saida_rubro_negro_crescente, "Crescente");
    // executar_testes_rubro_negro(Diretorio_entrada_decrescente, Diretorio_saida_rubro_negro_decrescente, "Decrescente");
    // executar_testes_rubro_negro(Diretorio_entrada_aleatorio, Diretorio_saida_rubro_negro_aleatorio, "Aleatório"); 

    executar_testes_23(Diretorio_entrada_crescente, Diretorio_saida_23_crescente, "Crescente");
    executar_testes_23(Diretorio_entrada_decrescente, Diretorio_saida_23_decrescente, "Decrescente");
    executar_testes_23(Diretorio_entrada_aleatorio, Diretorio_saida_23_aleatorio, "Aleatório");

    return 0;
}