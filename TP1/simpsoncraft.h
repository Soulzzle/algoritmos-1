#ifndef __SIMPSONCRAFT_H__
#define __SIMPSONCRAFT_H__

#include <stdbool.h>

#define MAX_BLOQUES 400
#define MAX_OBJETOS 50
#define MAX_FILAS 30
#define MAX_COLUMNAS 15

typedef struct coordenada {
    int fil;
    int col;
} coordenada_t;

typedef struct personaje {
    char tipo;
    coordenada_t posicion;
    int cantidad_madera;
    int cantidad_sopletes;
    bool recolecto_receta;
} personaje_t;

typedef struct bloque {
    char tipo;
    coordenada_t posicion;
    int resistencia;
} bloque_t;

typedef struct objeto {
    char tipo;
    coordenada_t posicion;
} objeto_t;

typedef struct juego {
    personaje_t homero;
    coordenada_t posicion_moe;
    coordenada_t posicion_receta;
    int movimientos_restantes;
    bloque_t bloques[MAX_BLOQUES];
    int tope_bloques;
    objeto_t herramientas[MAX_OBJETOS];
    int tope_herramientas;
    objeto_t obstaculos[MAX_OBJETOS];
    int tope_obstaculos;
} juego_t;

/*
 * Pre condiciones: -
 * Post condiciones: Inicializará el juego, cargando toda la información inicial de Homero, Moe, los bloques, las herramientas y los obstáculos.
 */
void inicializar_juego(juego_t *juego);


/*
 * Pre condiciones: El juego debe estar inicializado previamente con `inicializar_juego` y la acción
 * debe ser válida.
 * Post condiciones: Realizará la acción recibida por parámetro actualizando el juego.
 */
void realizar_jugada(juego_t *juego, char movimiento);

/*
 * Pre condiciones: El juego debe estar inicializado previamente con `inicializar_juego `.
 * Post condiciones: Imprime el juego por pantalla.
 */
void mostrar_juego(juego_t juego);

/*
 * Pre condiciones: El juego deberá estar inicializado previamente con `inicializar_juego `
 * Post condiciones: Devuelve:
 * --> 1 si es ganado
 * --> -1 si es perdido
 * --> 0 si se sigue jugando
 * El juego se dará por ganado cuando Homero recolecta su receta secreta y llega a Moe.
 * Se dará por perdido si se le terminan los movimientos antes de recolectar la receta y llegar a Moe.
 */
int estado_juego(juego_t juego);



#endif /* __SIMPSONCRAFT_H__ */
