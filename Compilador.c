#include <stdio.h>
#include <stdlib.h>

int main()
{

    int resultado = system("gcc -o Executavel ../main.c ../Teste_Rubro_negro/Teste_Rubro_negro.c ../Teste_Rubro_negro/Rubro_negro.c ../Teste_2-3/Teste_2-3.c ../Teste_2-3/2-3.c ../Teste_4-5/Teste_4-5.c ../Teste_4-5/4-5.c  ../Utilitarios/Cronometro.c");

    if (resultado == 0)
    {
        #ifdef _WIN32
            system("Executavel.exe");
        #else
            system("./Executavel");
        #endif
    }

    return 0;
}