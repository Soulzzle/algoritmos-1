#ifndef __GENERADOR_H__
#define __GENERADOR_H__

#include <stdbool.h>

#define MAX_NOMBRE 100
#define MAX_APELLIDO 100

typedef struct persona {
  char nombre[MAX_NOMBRE];
  char apellido[MAX_APELLIDO];
} persona_t;

typedef struct generador generador_t;

// Pre: -
//
// Post: Crea un TDA generador en el heap y devuelve un puntero
// al mismo. Devuelve NULL si no lo pudo crear.
generador_t *generador_crear();

// Pre: `generador` fue previamente creado con `generar_crear`.
//
// Post: Agrega una persona nueva a la lista negra del generador. A partir de
// que es agregada, tiene una chance de ser devuelta por el generador de
// nombres. Devuelve `true` si se pudo agregar o `false` si falló.
bool agregar_persona_lista_negra(generador_t *generador, persona_t persona);

// Pre: `generador` fue previamente creado con `generador_crear`.
//
// Post: Devuelve una persona aleatoria, la misma puede ser parte de la lista
// negra o ser una persona con un nombre aleatoriamente generado.
persona_t persona_aleatoria(generador_t *generador);

// Pre: El generador recibido fue previamente creado con `generador_crear`.
// Post: Libera la memoria que se reservó en el heap para el generador.
void generador_destruir(generador_t *generador);

#endif /* __GENERADOR_H__ */
