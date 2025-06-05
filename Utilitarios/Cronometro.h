#include <time.h>




#ifndef CRONOMETRO_H
#define CRONOMETRO_H

clock_t cronometro_iniciar();


clock_t cronometro_finalizar(clock_t inicio);



long double converter_para_segundos(clock_t tempo);

long double converter_para_milisegundos(clock_t tempo);

long double converter_para_microsegundos(clock_t tempo);

long double converter_para_nanosegundos(clock_t tempo);

char *converter_para_string(clock_t tempo, char *buffer, size_t tamanho_buffer, double (*converter)(clock_t));

#endif