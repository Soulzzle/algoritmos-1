#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "simpsoncraft.h"

const int VALOR_SIGUE_JUGANDO=0;
const int VALOR_JUEGO_GANADO=1;
const int VALOR_JUEGO_PERDIDO=-1;
const char LETRA_MOVERSE_DER='D';
const char LETRA_MOVERSE_IZQ='A';
const char LETRA_MOVERSE_ABAJO='S';
const char LETRA_USAR_ESCALERA='E';
const char LETRA_USAR_SOPLETE='O';

/*
 * Pre condiciones: -
 * Pos condiciones: Valida si el movimiento recibido por parámetro está entre los 5 movimientos válidos.
*/
bool es_mov_valido(char movimiento){
    bool es_mov_valido=true;
    if(movimiento!=LETRA_MOVERSE_DER && movimiento!=LETRA_MOVERSE_IZQ && movimiento!=LETRA_MOVERSE_ABAJO 
       && movimiento!=LETRA_USAR_ESCALERA && movimiento!=LETRA_USAR_SOPLETE)
        es_mov_valido=false;
    return es_mov_valido;
}

/*
 * Pre condiciones: -
 * Pos condiciones: Pregunta al usuario su siguiente movimiento y lo guarda en el movimiento pasado por parámetro.
*/
void preguntar_mov_usuario(char* movimiento){
    printf("Ingresa tu movimiento: ");
    scanf(" %c", movimiento);
    while(!es_mov_valido(*movimiento)){
        printf("Movimiento inválido, ingresa un movimiento disponible: ");
        scanf(" %c", movimiento);
    }
}

/*
 * Pre condiciones: -
 * Pos condiciones: Imprime por pantalla la bienvenida al juego.
*/
void imprimir_bienvenida(){
    printf("\n\x1b[42;97m- Bienvenido a Simpsoncraft -\x1b[0m\n");
    printf("Tu objetivo es conseguir la receta secreta 'R' y llegar hasta Moe 'M'\nsin quedarte sin movimientos. ¡Suerte!\n\n");
}

/*
 * Pre condiciones: El juego debe haber sido inicializado.
 * Pos condiciones: Le pregunta al usuario su siguiente movimiento, limpia la consola, pasa por parámetro el movimiento a realizar_jugada y llama a mostrar_juego.
*/
void ejecutar_juego(juego_t *juego, char movimiento){
    preguntar_mov_usuario(&movimiento);
    system("clear");
    realizar_jugada(juego, movimiento);
    mostrar_juego(*juego);
}

/*
 * Pre condiciones: -
 * Pos condiciones: Da la bienvenida, inicializa el juego, lo muestra, y lo ejecuta por primera vez usando ejecutar_juego.
*/
void dar_bienvenida_a_juego(juego_t *juego, char* movimiento){
    imprimir_bienvenida();
    inicializar_juego(juego);
    realizar_jugada(juego, *movimiento);
    mostrar_juego(*juego);
    ejecutar_juego(juego, *movimiento);
}

/*
 * Pre condiciones: El juego debe haber sido inicializado.
 * Pos condiciones: Imprime por pantalla, dependiendo del resultado del juego, un mensaje por haber ganado o perdido.
*/
void terminar_juego(juego_t juego){
    if(estado_juego(juego)==VALOR_JUEGO_GANADO)
        printf("\n\x1b[42;97mPudiste llegar a Moe con la receta, ¡Ganaste!\x1b[0m\n");
    else if(estado_juego(juego)==VALOR_JUEGO_PERDIDO)
        printf("\n\x1b[41;97mTe quedaste sin movimientos, perdiste :(\x1b[0m\n");
}

int main(){
    srand((unsigned) time(NULL));
    juego_t juego;
    char movimiento=' ';
    dar_bienvenida_a_juego(&juego, &movimiento);
    while(estado_juego(juego)==VALOR_SIGUE_JUGANDO){
        ejecutar_juego(&juego, movimiento);
    }
    terminar_juego(juego);
    return 0;
}