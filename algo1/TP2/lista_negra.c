#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "generador.h"

#define MAX_NOMBRE_JUGADOR 500
const char* LEER_ARCHIVO = "r";
const char* ESCRIBIR_ARCHIVO = "w";
const char* FORMATO_LECTURA_LISTA = "%[^;];%[^\n]\n";
const char* FORMATO_LECTURA_RANKING = "%[^;];%d;%d\n";
const char* NOMBRE_ARCHIVO_AUXILIAR = "ranking_auxiliar";
const char AFIRMATIVO = 'S';
const char NEGATIVO = 'N';
const int ARGC_ESPERADO = 3;
const int ERROR_LECTURA = 1;
const int ERROR_MEMORIA = 2;
const int ERROR_APERTURA = 3;
const int ERROR_CANT_ARGUMENTOS = 4;
const int EXITO = 0;
const int PUNTOS_BUENA_RESPUESTA = 1;
const int PUNTOS_MALA_RESPUESTA = -1;
const int PUNTOS_BONUS = 3;

typedef struct partida {
    char nombre_jugador[MAX_NOMBRE_JUGADOR];
    int largo_lista;
    int puntaje;
} partida_t;

/*
 * Precondiciones: Los tres archivos tienen que haber sido abiertos sin errores, o recibirse NULL.
 * Poscondiciones: Cierra los únicos tres archivos usados a lo largo del programa.
*/
void cerrar_archivos(FILE* archivo_lista, FILE* archivo_ranking, FILE* archivo_aux){
    if(archivo_lista){
        fclose(archivo_lista);
    }
    if(archivo_ranking){
        fclose(archivo_ranking);
    }
    if(archivo_aux){
        fclose(archivo_aux);
    }
}

/*
 * Precondiciones: -
 * Poscondiciones: Valida si el puntaje de partida1 es mayor que el de partida2.
*/
bool es_puntaje_mayor(partida_t partida1, partida_t partida2){
    bool respuesta = false;
    if(partida1.puntaje > partida2.puntaje){
        respuesta = true;
    }
    return respuesta;
}

/*
 * Precondiciones: -
 * Poscondiciones: Valida si el largo de lista de partida1 es mayor que el de partida2.
*/
bool es_lista_mayor(partida_t partida1, partida_t partida2){
    bool respuesta = false;
    if(partida1.largo_lista > partida2.largo_lista){
        respuesta = true;
    }
    return respuesta;
}

/*
 * Precondiciones: el campo nombre_jugador de los structs partida_t recibidos tienen que ser strings.
 * Poscondiciones: Valida si el primer caracter diferente de partida1 respecto a partida2 es mayor.
*/
bool es_nombre_mayor(partida_t partida1, partida_t partida2){
    bool respuesta = false;
    if(strcmp(partida1.nombre_jugador, partida2.nombre_jugador) > 0){
        respuesta = true;
    }
    return respuesta;
}

/*
 * Precondiciones: -
 * Poscondiciones: Valida si las partidas se encuentran ordenadas según los criterios de puntaje, lista y nombre.
*/
bool estan_partidas_ordenadas(partida_t partida1, partida_t partida2){
    bool respuesta = true;
    if(!es_puntaje_mayor(partida1, partida2)){
        if(partida1.puntaje != partida2.puntaje){
            respuesta = false;
        } else{
            if(!es_lista_mayor(partida1, partida2)){
                if(partida1.largo_lista != partida2.largo_lista){
                    respuesta = false;
                } else{
                    if(es_nombre_mayor(partida1, partida2)){
                        respuesta = false;
                    }
                }
            }
        }
    }
    return respuesta;
}

/*
 * Precondiciones: archivo_ranking abierto en modo lectura.
 * Poscondiciones: Lee una linea del archivo_ranking y guarda sus contenidos en partida_t partida.
*/
int leer_partida(FILE* archivo_ranking, partida_t* partida){
    return fscanf(archivo_ranking, FORMATO_LECTURA_RANKING, partida->nombre_jugador, &partida->largo_lista, &partida->puntaje);
}

/*
 * Precondiciones: archivo_ranking abierto en modo lectura.
 * Poscondiciones: Valida si el contenido del archivo se encuentra ordenado según los criterios de puntaje, lista y nombre.
*/
bool esta_ranking_ordenado(FILE* archivo_ranking){
    bool respuesta = true;
    partida_t partida1;
    partida_t partida2;
    leer_partida(archivo_ranking, &partida1); 
    int linea_siguiente = leer_partida(archivo_ranking, &partida2);
    while(linea_siguiente != EOF && respuesta){
        respuesta = estan_partidas_ordenadas(partida1, partida2);
        partida1 = partida2;
        linea_siguiente = fscanf(archivo_ranking, FORMATO_LECTURA_RANKING, partida2.nombre_jugador, &partida2.largo_lista, &partida2.puntaje);
    }
    return respuesta;
}

/*
 * Precondiciones: Vector lista correctamente inicializado y cant_personas mayor o igual que 0.
 * Poscondiciones: Imprime por pantalla todas las personas de la lista negra.
*/
void imprimir_lista(persona_t* lista, int cant_personas){
    for(int i = 0; i < cant_personas; i++){
        printf("\x1b[40;33m%s, %s.\x1b[0m\n", lista[i].nombre, lista[i].apellido);
    }
}

/*
 * Precondiciones: Vector lista correctamente inicializado y cant_personas mayor o igual que 0.
 * Poscondiciones: Limpia la pantalla e imprime en ella una bienvenida y explicación del juego.
*/
void imprimir_bienvenida(persona_t* lista, int cant_personas){
    system("clear");
    printf("\x1b[44;97m- Bienvenido al juego 'La Lista Negra' -\x1b[0m\n");
    printf("Tu trabajo es cuidar la entrada del bar de Moe, conociendo la Lista Negra: \n\n");
    imprimir_lista(lista, cant_personas);
    printf("\nMemorizá los nombres antes de que empiecen a ingresar las personas y...\n");
    printf("\x1b[41;97mNo dejes pasar a nadie de la Lista Negra.\x1b[0m  ('S' = Sí | 'N' = No)\n");
}

/*
 * Precondiciones: archivo_lista abierto en modo lectura, el vector lista_negra es válido, primera línea ya leída.
 * Poscondiciones: Agranda el vector lista_negra con realloc y guarda en él a persona_actual.
                   Devuelve EOF si llegó al final o 2 si este no es el caso.
*/
int devuelve_nombre_leido(FILE* archivo_lista, persona_t** lista_negra, int* cant_personas, bool* error_realloc, persona_t* persona_actual){
    int pos_actual = (*cant_personas);
    (*cant_personas)++;
    persona_t* lista_auxiliar = realloc(*lista_negra, ((unsigned long)(*cant_personas) * sizeof(persona_t)));
    if(lista_auxiliar == NULL){
        free(*lista_negra);
        (*lista_negra) = NULL;
        (*error_realloc) = true;
    } else{
        (*lista_negra) = lista_auxiliar;
        (*lista_negra)[pos_actual] = (*persona_actual);
    }
    return fscanf(archivo_lista, FORMATO_LECTURA_LISTA, persona_actual->nombre, persona_actual->apellido);
}

/*
 * Precondiciones: archivo_lista abierto en modo lectura.
 * Poscondiciones: Recorre archivo_lista guardando su contenido en un vector lista_negra dinámico, devuelve un puntero de este.
*/
persona_t* devuelve_puntero_lista(FILE* archivo_lista, int* cant_personas){
    bool error_realloc = false;
    persona_t persona_actual;
    persona_t* lista_negra = malloc(sizeof(persona_t));
    if(!lista_negra){
        return NULL;
    }
    int nombre_leido = fscanf(archivo_lista, FORMATO_LECTURA_LISTA, persona_actual.nombre, persona_actual.apellido);
    while(nombre_leido != EOF && !error_realloc){
        nombre_leido = devuelve_nombre_leido(archivo_lista, &lista_negra, cant_personas, &error_realloc, &persona_actual);
    }
    return lista_negra;
}

/*
 * Precondiciones: -
 * Poscondiciones: Pregunta al usuario su nombre y lo guarda en la variable nombre_jugador. Luego limpia la pantalla.
*/
void iniciar_juego(char nombre_jugador[MAX_NOMBRE_JUGADOR]){
    printf("\n\x1b[40;37;5mIngresa tu nombre para empezar a jugar: \x1b[0m");
    scanf(" %[^\n]", nombre_jugador);
    system("clear");
}

/*
 * Precondiciones: Vector lista correctamente inicializado y cant_personas mayor o igual que 0.
 * Poscondiciones: Carga un generador con todas las personas de la lista negra y devuelve un puntero al mismo.
*/
generador_t* cargar_generador(int cant_personas, persona_t* lista_negra){
    generador_t* generador = generador_crear();
    int i = 0;
    while(i < cant_personas){
        if(agregar_persona_lista_negra(generador, lista_negra[i])){
            i++;
        }
    }
    return generador;
}

/*
 * Precondiciones: los campos nombre y apellido del struct persona_t recibido tienen que ser strings.
 * Poscondiciones: Pregunta al usuario y valida que su respuesta sea solo 'S' o 'N'.
*/
void preguntar_jugador(char* respuesta, persona_t persona){
    printf("'%s %s' quiere pasar al bar, ¿Lo permitís?: ", persona.nombre, persona.apellido);
    scanf(" %c", respuesta);
    while((*respuesta) != AFIRMATIVO && (*respuesta) != NEGATIVO){
        printf("Respuesta inválida, elegí entre sí ('S') y no ('N'): ");
        scanf(" %c", respuesta);
    }
}

/*
 * Precondiciones: los campos nombre y apellido de ambos structs persona_t tienen que ser strings.
 * Poscondiciones: Valida si los campos nombre y apellido de ambas personas son iguales.
*/
bool son_misma_persona(persona_t persona1, persona_t persona2){
    bool respuesta = false;
    if((strcmp(persona1.nombre, persona2.nombre) == 0) && (strcmp(persona1.apellido, persona2.apellido) == 0)){
        respuesta = true;
    }
    return respuesta;
}

/*
 * Precondiciones: Vector lista correctamente inicializado y cant_personas mayor o igual que 0.
 * Poscondiciones: Valida si la persona recibida por parámetro se encuentra en la lista negra usando son_misma_persona.
*/
bool esta_en_lista_negra(persona_t persona, persona_t* lista_negra, int cant_personas){
    bool respuesta = false;
    for(int i = 0; i < cant_personas && !respuesta; i++){
        if(son_misma_persona(persona, lista_negra[i])){
            respuesta = true;
        }
    }
    return respuesta;
}

/*
 * Precondiciones: Vector lista correctamente inicializado y cant_personas mayor o igual que 0.
 * Poscondiciones: Devuelve el puntaje del usuario en la ronda y actualiza el valor bool pasado por parámetro en caso de perder.
*/
int devuelve_puntaje_ronda(char respuesta, persona_t persona, persona_t* lista_negra, int cant_personas, bool* perdio_jugador){
    int puntaje_ronda = 0;
    if(respuesta == AFIRMATIVO && !esta_en_lista_negra(persona, lista_negra, cant_personas)){
        puntaje_ronda += PUNTOS_BUENA_RESPUESTA;
    } else if(respuesta == NEGATIVO && !esta_en_lista_negra(persona, lista_negra, cant_personas)){
        puntaje_ronda += PUNTOS_MALA_RESPUESTA;
    } else if(respuesta == NEGATIVO && esta_en_lista_negra(persona, lista_negra, cant_personas)){
        puntaje_ronda += PUNTOS_BONUS;
    } else{
        (*perdio_jugador) = true;
    }
    return puntaje_ronda;
}

/*
 * Precondiciones: Vector lista correctamente inicializado y cant_personas mayor o igual que 0.
 * Poscondiciones: Crea un generador y lo carga con cargar_generador, usa el mismo para ejecutar el 
                   juego en loop hasta que perdio_jugador sea true. Por último, destruye el generador.
*/
void ejecutar_juego(int cant_personas, persona_t* lista_negra, int* puntaje, char nombre_jugador[MAX_NOMBRE_JUGADOR]){
    char respuesta;
    bool perdio_jugador = false;
    persona_t persona;
    iniciar_juego(nombre_jugador);
    generador_t* generador = cargar_generador(cant_personas, lista_negra);
    while(!perdio_jugador){
        persona = persona_aleatoria(generador);
        preguntar_jugador(&respuesta, persona);
        (*puntaje) += devuelve_puntaje_ronda(respuesta, persona, lista_negra, cant_personas, &perdio_jugador);
    }
    generador_destruir(generador);
} 

/*
 * Precondiciones: nombre_jugador tiene que ser un string válido.
 * Poscondiciones: Devuelve un struct tipo partida_t con puntaje, cant_personas y nombre_jugador dados.
*/
partida_t devuelve_partida(int cant_personas, int puntaje, char nombre_jugador[MAX_NOMBRE_JUGADOR]){
    partida_t partida;
    strcpy(partida.nombre_jugador, nombre_jugador);
    partida.puntaje = puntaje;
    partida.largo_lista = cant_personas;
    return partida;
}

/*
 * Precondiciones: archivo tiene que estar abierto en modo escritura y partida.nombre_jugador ser un string.
 * Poscondiciones: Imprime en el archivo recibido por parámetro la partida recibida.
*/
void insertar_partida(FILE* archivo, partida_t partida){
    fprintf(archivo, "%s;%i;%i\n", partida.nombre_jugador, partida.largo_lista, partida.puntaje);
}

/*
 * Precondiciones: archivo_aux abierto en modo escritura.
 * Poscondiciones: Compara la partida_actual con partida_jugada y copia en archivo_aux sus contenidos. 
*/
void mover_partidas(FILE* archivo_aux, int* contador_lineas, partida_t partida_actual, partida_t partida_jugada, bool* partida_insertada, int* puesto){
    (*contador_lineas)++;
    if(!(*partida_insertada) && !estan_partidas_ordenadas(partida_actual, partida_jugada)){
        insertar_partida(archivo_aux, partida_jugada);
        insertar_partida(archivo_aux, partida_actual);
        (*partida_insertada) = true;
        (*puesto) = (*contador_lineas);
    } else{
        insertar_partida(archivo_aux, partida_actual);
    }
}

/*
 * Precondiciones: archivo_aux abierto en modo escritura.
 * Poscondiciones: Copia en archivo_aux el contenido de archivo_ranking mas partida_jugada, de forma ordenada.
*/
void mover_ranking(FILE* archivo_aux, char* nombre_archivo_ranking, partida_t partida_jugada, int* puesto, bool* error_archivo){
    FILE* archivo_ranking = fopen(nombre_archivo_ranking, LEER_ARCHIVO);
    if(!archivo_ranking){
        (*error_archivo) = true;
        return;
    }
    bool partida_insertada = false;
    int contador_lineas = 0;
    partida_t partida_actual;
    int linea_actual = leer_partida(archivo_ranking, &partida_actual);
    while(linea_actual != EOF){
        mover_partidas(archivo_aux, &contador_lineas, partida_actual, partida_jugada, &partida_insertada, puesto);
        linea_actual = leer_partida(archivo_ranking, &partida_actual);
    }
    if(!partida_insertada){
        contador_lineas++;
        insertar_partida(archivo_aux, partida_jugada);
        (*puesto) = contador_lineas;
    }
}

/*
 * Precondiciones: -
 * Poscondiciones: Imprime por pantalla un mensaje indicando que terminó el juego, junto con el puntaje y ranking del usuario.
*/
void imprimir_despedida(int puntaje, int puesto){
    printf("¡Dejaste pasar al bar una persona de la lista negra, perdiste!\n");
    printf("Puntaje: %i | Puesto en ranking: %i\n\n", puntaje, puesto);
}

int main(int argc, char* argv[]){
    if(argc != ARGC_ESPERADO){
        return ERROR_CANT_ARGUMENTOS;
    }
    char* nombre_archivo_lista = argv[1];
    char* nombre_archivo_ranking = argv[2];
    FILE* archivo_lista = fopen(nombre_archivo_lista, LEER_ARCHIVO);
    if(!archivo_lista){
        return ERROR_APERTURA;
    }
    FILE* archivo_ranking = fopen(nombre_archivo_ranking, LEER_ARCHIVO);
    if(!archivo_ranking){
        cerrar_archivos(archivo_lista, NULL, NULL);
        return ERROR_APERTURA;
    }
    FILE* archivo_aux = fopen(NOMBRE_ARCHIVO_AUXILIAR, ESCRIBIR_ARCHIVO);
    if(!archivo_aux){
        cerrar_archivos(archivo_lista, archivo_ranking, NULL);
        return ERROR_APERTURA;
    }
    if(!esta_ranking_ordenado(archivo_ranking)){
        cerrar_archivos(archivo_lista, archivo_ranking, archivo_aux);
        return ERROR_LECTURA;
    }
    int cant_personas = 0;
    int puntaje = 0;
    char nombre_jugador[MAX_NOMBRE_JUGADOR];
    int puesto = 0;
    bool error_archivo = false;
    persona_t* lista_negra = devuelve_puntero_lista(archivo_lista, &cant_personas);
    if(lista_negra == NULL || cant_personas == 0){
        cerrar_archivos(archivo_lista, archivo_ranking, archivo_aux);
        return ERROR_MEMORIA;
    }
    imprimir_bienvenida(lista_negra, cant_personas);
    ejecutar_juego(cant_personas, lista_negra, &puntaje, nombre_jugador);
    free(lista_negra);
    partida_t partida_jugada = devuelve_partida(cant_personas, puntaje, nombre_jugador);
    mover_ranking(archivo_aux, nombre_archivo_ranking, partida_jugada, &puesto, &error_archivo);
    if(error_archivo){
        cerrar_archivos(archivo_lista, NULL, archivo_aux);
        return ERROR_APERTURA;
    }
    cerrar_archivos(archivo_lista, archivo_ranking, archivo_aux);
    remove(nombre_archivo_ranking);
    rename(NOMBRE_ARCHIVO_AUXILIAR, nombre_archivo_ranking);
    imprimir_despedida(puntaje, puesto);
    return EXITO;
}