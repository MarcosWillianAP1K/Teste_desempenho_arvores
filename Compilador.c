#include <stdio.h>
#include <stdlib.h>

int main()
{

    int resultado = system("gcc -o Executavel ../main.c ../Teste_Rubro_negro/Teste_Rubro_negro.c ../Teste_Rubro_negro/Rubro_negro.c  ../Utilitarios/Cronometro.c ../Utilitarios/Manipular_arquivo.c");

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