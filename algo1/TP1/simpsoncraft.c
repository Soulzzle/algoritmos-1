#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "simpsoncraft.h"
#include "utiles.h"

const char LETRA_HOMERO='H';
const char LETRA_MOE='M';
const char LETRA_RECETA='R';
const char LETRA_VIGA='V';
const char LETRA_TIERRA='T';
const char LETRA_TELARANIA='A';
const char LETRA_MADERA='W';
const char LETRA_SOPLETE='S';
const char LETRA_CABLE='C';
const char LETRA_ESCALERA='E';
const char LETRA_MURCIELAGO='B';
const char ESPACIO_VACIO=' ';
const char LETRA_INVALIDA='-';
const int FIL_INICIO=0;
const int COL_INICIO=0;
const int FIL_FINAL=29;
const int COL_FINAL=14;
const int POS_INVALIDA=-1;
const int CANT_MADERA_INICIO=15;
const int CANT_SOPLETE_INICIO=2;
const int CANT_MOV_INICIO=100;
const int CANT_MOV_NULOS=0;
const int CANT_TIERRAS=80;
const int CANT_TELARANIAS=100;
const int CANT_MADERAS=20;
const int CANT_SOPLETES=10;
const int CANT_CABLES=25;
const int CANT_MURCIELAGOS=20;
const int CANT_RECUPERAR_MADERAS=2;
const int RESIST_VIGA=-1;
const int RESIST_TIERRA=2;
const int RESIST_TELARANIA=1;
const int RESIST_ESCALERA=1;
const int RESTA_POR_CABLE=5;
const int RESTA_POR_MURCIELAGOS=3;
const int COSTO_ESCALERA=3;
const int COSTO_SOPLETE=1;
const int COSTO_MOVIMIENTO=1; 
const int RADIO_SOPLETE=2;
const char MOVERSE_DER='D';
const char MOVERSE_IZQ='A';
const char MOVERSE_ABAJO='S';
const char USAR_ESCALERA='E';
const char USAR_SOPLETE='O';
const int SIGUE_JUGANDO=0;
const int JUEGO_GANADO=1;
const int JUEGO_PERDIDO=-1;

/*
 * Pre condiciones: -
 * Pos condiciones: Inicializa a Homero con toda su información inicial.
*/
void inicializar_homero(juego_t *juego){
    juego->homero.tipo=LETRA_HOMERO;
    juego->homero.posicion.fil=FIL_INICIO;
    juego->homero.posicion.col=COL_INICIO;
    juego->homero.cantidad_madera=CANT_MADERA_INICIO;
    juego->homero.cantidad_sopletes=CANT_SOPLETE_INICIO;
    juego->homero.recolecto_receta=false;
}

/*
* Pre condiciones: -
* Pos condiciones: Inicializa todas las vigas del juego con su respectiva información inicial.
*/
void inicializar_vigas(juego_t *juego){
    coordenada_t vigas[MAX_VIGAS];
    int tope_vigas=0;
    obtener_mapa(vigas,&tope_vigas);
    for(int i=0;i<tope_vigas;i++){
        juego->bloques[i].tipo=LETRA_VIGA;
        juego->bloques[i].posicion=vigas[i];
        juego->bloques[i].resistencia=RESIST_VIGA;
        juego->tope_bloques++;
    }
}

/*
 * Pre condiciones: Las posiciones deben estar dentro del rango del terreno.
 * Pos condiciones: Valida si las posiciones recibidas por parámetro son iguales.
*/
bool son_pos_iguales(coordenada_t pos1, coordenada_t pos2){
    bool son_iguales=false;
    if(pos1.fil==pos2.fil && pos1.col==pos2.col){
        son_iguales=true;
    }
    return son_iguales;
}

/*
 * Pre condiciones: Las posiciones de Homero, Moe y la receta, al igual que el tope de bloques, deben estar ya inicializados.
 * Pos condiciones: Valida si la posición pasada por parámetro está ocupada.
*/
bool valida_pos_ocupada(juego_t juego, coordenada_t pos){ //se romperá esta función al romperse un bloque?
    bool pos_ocupada=false;
    int i=0;
    if((son_pos_iguales(juego.homero.posicion, pos)) || (son_pos_iguales(juego.posicion_moe, pos)) || (son_pos_iguales(juego.posicion_receta, pos))){
        pos_ocupada=true;
    }
    while(!pos_ocupada && i<juego.tope_bloques){
        if(son_pos_iguales(juego.bloques[i].posicion, pos)){
            pos_ocupada=true;
        }
        i++;
    }
    return pos_ocupada;
}

/*
 * Pre condiciones: -
 * Pos condiciones: Inicializa la posición de Moe.
*/
void inicializar_moe(juego_t *juego){
    juego->posicion_moe.fil=POS_INVALIDA;
    juego->posicion_moe.col=POS_INVALIDA;
    coordenada_t pos;
    do{
        pos.fil=FIL_FINAL;
        pos.col=rand()%MAX_COLUMNAS;
    } while(valida_pos_ocupada(*juego, pos));
    juego->posicion_moe=pos;
}

/*
 * Pre condiciones: -
 * Pos condiciones: Genera una fila y columna random en la posición pasada por parámetro.
*/
void generar_pos_bloque(juego_t juego, coordenada_t *pos){
    do{
        pos->fil=rand()%MAX_FILAS;
        pos->col=rand()%MAX_COLUMNAS;
    } while(valida_pos_ocupada(juego, *pos));
}

/*
 * Pre condiciones: El tope de bloques tiene que estar correctamente inicializado.
 * Pos condiciones: Inicializa un bloque, con la información recibida por parámetro, en el final del vector bloques y aumenta su tope.
*/
void inicializa_bloque(juego_t *juego, char tipo, int resistencia){
    int indice=juego->tope_bloques;
    juego->bloques[indice].tipo=tipo;
    juego->bloques[indice].resistencia=resistencia;
    generar_pos_bloque(*juego, &(juego->bloques[indice].posicion));
    juego->tope_bloques++;
}

/*
 * Pre condiciones: El tope de bloques tiene que estar correctamente inicializado.
 * Pos condiciones: Inicializa todas las tierras y telarañas del juego con su respectiva información inicial.
*/
void inicializar_bloques_restantes(juego_t *juego){
    int tope_bloques=juego->tope_bloques;
    for(int i=juego->tope_bloques;i<(tope_bloques+CANT_TIERRAS+CANT_TELARANIAS);i++){
        if(i<(tope_bloques+CANT_TIERRAS))
            inicializa_bloque(juego, LETRA_TIERRA, RESIST_TIERRA);
        else
            inicializa_bloque(juego, LETRA_TELARANIA, RESIST_TELARANIA);
    }
}

/*
 * Pre condiciones: Los topes de objetos deben estar correctamente inicializados.
 * Pos condiciones: Valida si ya hay un objeto en la posición del bloque dada por parámetro.
*/
bool valida_bloque_ocupado(juego_t juego, coordenada_t pos){
    bool esta_bloque_ocupado=false;
    for (int i=0;i<juego.tope_herramientas && !esta_bloque_ocupado;i++){
        if (son_pos_iguales(juego.herramientas[i].posicion, pos)){
            esta_bloque_ocupado=true;
        }
    }
    for (int i=0;i<juego.tope_obstaculos && !esta_bloque_ocupado;i++){
        if (son_pos_iguales(juego.obstaculos[i].posicion, pos)){
            esta_bloque_ocupado=true;
        }
    }
    return esta_bloque_ocupado;
}

/*
 * Pre condiciones: El tope y vector de bloques debe estar correctamente inicializado.
 * Pos condiciones: Genera una posición basada en un bloque ya existente aleatorio.
*/
void generar_pos_objeto(juego_t *juego, coordenada_t *pos){
    int indice_random=-1;
    do{
        indice_random=rand()%(juego->tope_bloques);
        *(pos)=juego->bloques[indice_random].posicion;
    } while(juego->bloques[indice_random].tipo==LETRA_VIGA || valida_bloque_ocupado(*juego, *pos));
}

/*
 * Pre condiciones: -
 * Pos condiciones: Inicializa un objeto y aumenta su respectivo tope.
*/
void inicializa_objeto(juego_t *juego, int indice, char tipo){
    if(tipo==LETRA_MADERA || tipo==LETRA_SOPLETE){
        juego->herramientas[indice].tipo=tipo;
        generar_pos_objeto(juego, &(juego->herramientas[indice].posicion));
        juego->tope_herramientas++;
    } else{
        juego->obstaculos[indice].tipo=tipo;
        generar_pos_objeto(juego, &(juego->obstaculos[indice].posicion));
        juego->tope_obstaculos++;
    }
}

/*
 * Pre condiciones: Los topes de herramientas y objetos tienen que estar inicializados.
 * Pos condiciones: Inicializa todos los objetos con su respectiva información inicial.
*/
void inicializar_objetos(juego_t *juego){
    int tope_herramientas=juego->tope_herramientas;
    int tope_obstaculos=juego->tope_obstaculos;
    for(int i=tope_herramientas;i<(tope_herramientas+CANT_MADERAS+CANT_SOPLETES);i++){
        if(i<(tope_herramientas+CANT_MADERAS))
            inicializa_objeto(juego, i, LETRA_MADERA);
        else
            inicializa_objeto(juego, i, LETRA_SOPLETE);
    }
    for(int i=tope_obstaculos;i<(tope_obstaculos+CANT_CABLES+CANT_MURCIELAGOS);i++){
        if(i<(tope_obstaculos+CANT_CABLES))
            inicializa_objeto(juego, i, LETRA_CABLE);
        else
            inicializa_objeto(juego, i, LETRA_MURCIELAGO);
    }
}

/*
 * Pre condiciones: -
 * Pos condiciones: Inicializa la posición de la receta.
*/
void inicializar_receta(juego_t *juego){
    juego->posicion_receta.fil=POS_INVALIDA;
    juego->posicion_receta.col=POS_INVALIDA;
    coordenada_t pos;
    do{
        pos.fil=rand()%MAX_FILAS;
        pos.col=rand()%MAX_COLUMNAS;
    } while(valida_pos_ocupada(*juego, pos));
    juego->posicion_receta=pos;
}

/*
 * Pre condiciones: -
 * Post condiciones: Inicializará el juego, cargando toda la información inicial de Homero, Moe, los bloques, las herramientas y los obstáculos.
 */
void inicializar_juego(juego_t *juego){ //homero, vigas, moe, tierras y telarañas, objetos, receta.
    juego->tope_bloques=0;
    juego->tope_herramientas=0;
    juego->tope_obstaculos=0;
    juego->movimientos_restantes=CANT_MOV_INICIO;
    inicializar_homero(juego);
    inicializar_vigas(juego);
    inicializar_moe(juego);
    inicializar_bloques_restantes(juego);
    inicializar_objetos(juego);
    inicializar_receta(juego);
}

/*
 * Pre condiciones: El juego debe estar correctamente inicializado y la posición debe estar dentro del terreno.
 * Pos condiciones: Devuelve la letra del elemento que se encuentra en la posición dada por parámetro.
*/
char devuelve_elemento_en_pos(juego_t juego, coordenada_t pos){ //modularizar esto
    
    char elemento=ESPACIO_VACIO;
    if(son_pos_iguales(juego.homero.posicion, pos)){
        elemento=LETRA_HOMERO;
    } else if(son_pos_iguales(juego.posicion_moe, pos)){
        elemento=LETRA_MOE;
    } else if(son_pos_iguales(juego.posicion_receta, pos)){
        elemento=LETRA_RECETA;
    }
    for(int i=0;i<juego.tope_bloques && elemento==ESPACIO_VACIO;i++){
         if(son_pos_iguales(juego.bloques[i].posicion, pos))
            elemento=juego.bloques[i].tipo;
    }
   for(int i=0;i<juego.tope_herramientas && elemento==ESPACIO_VACIO;i++){
        if(son_pos_iguales(juego.herramientas[i].posicion, pos)){
            elemento=juego.herramientas[i].tipo;
        }
   }
    for(int i=0;i<juego.tope_obstaculos && elemento==ESPACIO_VACIO;i++){
        if(son_pos_iguales(juego.obstaculos[i].posicion, pos)){
            elemento=juego.obstaculos[i].tipo;
        }
    }
    return elemento;
}

/*
 * Pre condiciones: El juego debe estar correctamente inicializado y matriz no nula.
 * Pos condiciones: Llena la matriz dada por parámetro con los elementos que están en cada posición.
*/
void llenar_terreno_juego(juego_t juego, char matriz[MAX_FILAS][MAX_COLUMNAS]){
    coordenada_t pos;
    for(int i=0;i<MAX_FILAS;i++){
        for(int j=0;j<MAX_COLUMNAS;j++){
            pos.fil=i;
            pos.col=j;
            matriz[i][j]=devuelve_elemento_en_pos(juego, pos);
        }
    }
}

/*
 * Pre condiciones: El juego debe estar correctamente inicializado y la matriz debe ser inicializada por "llenar_terreno_juego".
 * Pos condiciones: Llena la matriz dada por parámetro con los elementos que están en cada posición.
*/
void imprimir_terreno_juego(char matriz[MAX_FILAS][MAX_COLUMNAS]){
    char* fondo="";
    char* sacar_fondo="\x1b[0m";
    for(int i=0;i<MAX_FILAS;i++){
        for(int j=0;j<MAX_COLUMNAS;j++){
            fondo="\x1b[40;0m";
            if(matriz[i][j]==LETRA_HOMERO)
                fondo="\x1b[43;30m";
            else if(matriz[i][j]==LETRA_RECETA)
                fondo="\x1b[42;30m";
            else if(matriz[i][j]==LETRA_MOE)
                fondo="\x1b[44;97m";
            else if(matriz[i][j]==LETRA_TIERRA)
                fondo="\x1b[48;5;94m";
            else if(matriz[i][j]==LETRA_VIGA)
                fondo="\x1b[100;97m";
            else if(matriz[i][j]==LETRA_TELARANIA)
                fondo="\x1b[47;30m";
            printf("%s %c", fondo, matriz[i][j]);
            printf("%s", sacar_fondo);
        }
        printf("\n");
    }
}

/*
 * Pre condiciones: El juego debe estar correctamente inicializado.
 * Pos condiciones: Muestra por pantalla el estado del juego.
*/
void imprimir_estado_juego(juego_t juego){
    int movimientos_restantes=juego.movimientos_restantes;
    int maderas_restantes=juego.homero.cantidad_madera;
    int sopletes_restantes=juego.homero.cantidad_sopletes;
    printf("\n-- Movimientos restantes:%i | Maderas restantes:%i | Sopletes restantes:%i --\n", 
        movimientos_restantes, 
        maderas_restantes, 
        sopletes_restantes);
    printf("Receta conseguida: ");
    if(juego.homero.recolecto_receta)
        printf("SI\n");
    else
        printf("NO\n");
}

/*
 * Pre condiciones: El juego debe estar inicializado previamente con `inicializar_juego `.
 * Post condiciones: Imprime el juego por pantalla.
 */
void mostrar_juego(juego_t juego){
    char terreno[MAX_FILAS][MAX_COLUMNAS];
    llenar_terreno_juego(juego, terreno);
    imprimir_terreno_juego(terreno);
    printf("\x1b[37mControles del juego: D:'Moverse a la derecha' | A:'Moverse a la izquierda'\nS:'Moverse abajo' | E:'Usar escalera' | O:'Usar soplete'\x1b[0m\n");
    imprimir_estado_juego(juego);
}

/*
 * Pre condiciones: -
 * Pos condiciones: Valida si la accion recibida por parámetro es un movimiento.
*/
bool es_movimiento(char accion){
    bool es_movimiento=false;
    if(accion==MOVERSE_IZQ || accion==MOVERSE_DER || accion==MOVERSE_ABAJO)
        es_movimiento=true;
    return es_movimiento;
}

/*
 * Pre condiciones: El juego debe estar correctamente inicializado.
 * Pos condiciones: Valida si el movimiento se encuentra dentro de los límites del terreno.
*/
bool es_movimiento_posible(juego_t juego, char accion){
    bool es_movimiento_posible=true;
    int fil_arriba=(juego.homero.posicion.fil-1);
    int fil_abajo=(juego.homero.posicion.fil+1);
    int col_izq=(juego.homero.posicion.col-1);
    int col_der=(juego.homero.posicion.col+1);
    if(accion==MOVERSE_IZQ)
        es_movimiento_posible=(col_izq>=COL_INICIO);
    else if(accion==MOVERSE_DER)
        es_movimiento_posible=(col_der<=COL_FINAL);
    else if(accion==MOVERSE_ABAJO)
        es_movimiento_posible=(fil_abajo<=FIL_FINAL);
    else if(accion==USAR_ESCALERA)
        es_movimiento_posible=(fil_arriba>=FIL_INICIO);
    return es_movimiento_posible;
}

/*
 * Pre condiciones: El juego debe estar correctamente inicializado.
 * Pos condiciones: Valida si hay un bloque en la dirección que se quiere mover el personaje y actualiza el índice recibido por parámetro.
*/
bool hay_bloque_en_direccion(juego_t juego, char accion, int* indice_bloque){
    bool hay_bloque=false;
    coordenada_t pos_izq={juego.homero.posicion.fil, juego.homero.posicion.col-1};
    coordenada_t pos_der={juego.homero.posicion.fil, juego.homero.posicion.col+1};
    coordenada_t pos_arriba={juego.homero.posicion.fil-1, juego.homero.posicion.col};
    coordenada_t pos_abajo={juego.homero.posicion.fil+1, juego.homero.posicion.col};
    int i=0;
    while(i<juego.tope_bloques && !hay_bloque){
        if((accion==MOVERSE_DER && son_pos_iguales(juego.bloques[i].posicion, pos_der))
            || (accion==MOVERSE_IZQ && son_pos_iguales(juego.bloques[i].posicion, pos_izq))
            || (accion==MOVERSE_ABAJO && son_pos_iguales(juego.bloques[i].posicion, pos_abajo))
            || (accion==USAR_ESCALERA && son_pos_iguales(juego.bloques[i].posicion, pos_arriba))){
            hay_bloque=true;
            *indice_bloque=i;
        }
        i++;
    }
    return hay_bloque;
}

/*
 * Pre condiciones: El juego debe estar correctamente inicializado.
 * Pos condiciones: Elimina o cambia de posición el bloque asociado al índice recibido por parámetro.
*/
void romper_bloque(juego_t *juego, int indice_bloque){
    int ultimo_bloque=(juego->tope_bloques-1);
    if(juego->bloques[indice_bloque].tipo==LETRA_ESCALERA){
        juego->homero.cantidad_madera+=CANT_RECUPERAR_MADERAS;
        printf("¡Recuperaste 2 maderas!\n");
    }
    if(juego->bloques[indice_bloque].tipo==LETRA_TELARANIA)
        generar_pos_bloque(*juego, &(juego->bloques[indice_bloque].posicion));
    else{
        for(int i=indice_bloque;i<ultimo_bloque;i++){
            juego->bloques[i]=juego->bloques[i+1];
        }
        juego->tope_bloques--;
    }
}

/*
 * Pre condiciones: El juego debe estar correctamente inicializado.
 * Pos condiciones: Mueve al personaje en la dirección recibida por parámetro.
*/
void mover_homero(juego_t *juego, char accion){
    if(accion==MOVERSE_IZQ)
        juego->homero.posicion.col--;
    else if(accion==MOVERSE_DER)
        juego->homero.posicion.col++;
    else if(accion==USAR_ESCALERA)
        juego->homero.posicion.fil--;
    juego->movimientos_restantes-=COSTO_MOVIMIENTO;
}

/*
 * Pre condiciones: El juego debe estar correctamente inicializado.
 * Pos condiciones: Valida si hay un bloque debajo del personaje.
*/
bool hay_bloque_abajo(juego_t juego){
    bool hay_bloque=false;
    int i=0;
    coordenada_t pos_abajo={juego.homero.posicion.fil+1, juego.homero.posicion.col};
    while(i<juego.tope_bloques && !hay_bloque){
        hay_bloque=son_pos_iguales(juego.bloques[i].posicion, pos_abajo);
        i++;
    }
    return hay_bloque;
}

/*
 * Pre condiciones: El juego debe estar correctamente inicializado.
 * Pos condiciones: Si el personaje está en la misma posición que la receta, actualiza el valor bool correspondiente y la elimina del terreno.
*/
void interactuar_receta(juego_t *juego){
    if(son_pos_iguales(juego->homero.posicion, juego->posicion_receta)){
        printf("¡Conseguiste la receta!\n");
        juego->homero.recolecto_receta=true;
        juego->posicion_receta.fil=POS_INVALIDA;
        juego->posicion_receta.col=POS_INVALIDA;
    }
}

/*
 * Pre condiciones: El juego debe estar correctamente inicializado.
 * Pos condiciones: Elimina la herramienta asociada al índice recibido por parámetro.
*/
void romper_herramientas(juego_t *juego, int indice){
    int ultima_herramienta=(juego->tope_herramientas-1);
    for(int i=indice;i<ultima_herramienta;i++){
        juego->herramientas[i]=juego->herramientas[i+1];
    }
    juego->tope_herramientas--;
}

/*
 * Pre condiciones: El juego debe estar correctamente inicializado.
 * Pos condiciones: Elimina el obstáculo asociado al índice recibido por parámetro.
*/
void romper_obstaculos(juego_t *juego, int indice){
    int ultimo_obstaculo=(juego->tope_obstaculos-1);
    for(int i=indice;i<ultimo_obstaculo;i++){
        juego->obstaculos[i]=juego->obstaculos[i+1];
    }
    juego->tope_obstaculos--;
}

/*
 * Pre condiciones: El juego debe estar correctamente inicializado y existir una herramienta en el índice dado.
 * Pos condiciones: Dependiendo del tipo de herramienta, modifica la cantidad de maderas/sopletes y luego elimina la herramienta del vector.
*/
void aplicar_y_eliminar_herramienta(juego_t *juego, int indice){
    if(juego->herramientas[indice].tipo==LETRA_MADERA){
        juego->homero.cantidad_madera++;
        printf("¡Encontraste una madera!\n");
    } else if(juego->herramientas[indice].tipo==LETRA_SOPLETE){
        juego->homero.cantidad_sopletes++;
        printf("¡Encontraste un soplete!\n");
    }
    romper_herramientas(juego, indice);
}

/*
 * Pre condiciones: El juego debe estar correctamente inicializado y existir un obstáculo en el índice dado.
 * Pos condiciones: Dependiendo del tipo de obstáculo, modifica la cantidad de movimientos/maderas y luego elimina el obstáculo del vector.
*/
void aplicar_y_eliminar_obstaculo(juego_t *juego, int indice){
    if(juego->obstaculos[indice].tipo==LETRA_CABLE){
        juego->movimientos_restantes-=RESTA_POR_CABLE;
        printf("Te chocaste con un cable, perdés %i movimientos :(\n", RESTA_POR_CABLE);
    } else if(juego->obstaculos[indice].tipo==LETRA_MURCIELAGO){
        if(juego->homero.cantidad_madera<=RESTA_POR_MURCIELAGOS){
            juego->homero.cantidad_madera-=juego->homero.cantidad_madera;
            printf("Te chocaste con murciélagos, perdés todas tus maderas :(\n");
        } else{
            juego->homero.cantidad_madera-=RESTA_POR_MURCIELAGOS;
            printf("Te chocaste con murciélagos, perdés %i maderas :(\n", RESTA_POR_MURCIELAGOS);
        }
    }
    romper_obstaculos(juego, indice);
}

/*
 * Pre condiciones: El juego debe estar correctamente inicializado.
 * Pos condiciones: Actualiza la cantidad de maderas/sopletes/movimientos dependiendo del objeto interactuado, eliminandolo posteriormente.
*/
void interactuar_objetos(juego_t *juego){
    int i=0;
    bool objeto_encontrado=false;
    while(i<juego->tope_herramientas && !objeto_encontrado){
        if(son_pos_iguales(juego->homero.posicion, juego->herramientas[i].posicion)){
            objeto_encontrado=true;
            aplicar_y_eliminar_herramienta(juego, i);
        }
        i++;
    }
    i=0;
    while(i<juego->tope_obstaculos && !objeto_encontrado){
        if(son_pos_iguales(juego->homero.posicion, juego->obstaculos[i].posicion)){
            objeto_encontrado=true;
            aplicar_y_eliminar_obstaculo(juego, i);
        }
        i++;
    }
}

/*
 * Pre condiciones: El juego debe estar correctamente inicializado.
 * Pos condiciones: Muestra un mensaje por pantalla dependiendo de si el personaje está en la misma posición que Moe.
*/
void interactuar_moe(juego_t juego){
    if(son_pos_iguales(juego.homero.posicion, juego.posicion_moe) && !juego.homero.recolecto_receta)
        printf("Todavía no conseguiste la receta secreta.\n");
}

/*
 * Pre condiciones: El juego debe estar correctamente inicializado.
 * Pos condiciones: Interactúa con todos los posibles elementos que puedan estar en la misma posición que el personaje.
*/
void interactuar_elementos(juego_t *juego){
    interactuar_receta(juego);
    interactuar_objetos(juego);
    interactuar_moe(*juego);
}

/*
 * Pre condiciones: El juego debe estar correctamente inicializado.
 * Pos condiciones: Suma 1 a la fila del personaje mientras que no haya un bloque abajo o no este ya en la última fila. 
*/
void caer_por_gravedad(juego_t *juego){
    while(juego->homero.posicion.fil<FIL_FINAL && !hay_bloque_abajo(*juego)){
        juego->homero.posicion.fil++;
        interactuar_elementos(juego);
    }
}

/*
 * Pre condiciones: El juego debe estar correctamente inicializado y la respectiva escalera debe haber sido creada.
 * Pos condiciones: Iguala la posición del personaje a la posición de arriba de la escalera.
*/
void interactuar_escaleras(juego_t *juego, int indice_escalera){
    coordenada_t pos_arriba={juego->bloques[indice_escalera].posicion.fil-1, juego->bloques[indice_escalera].posicion.col};
    while(devuelve_elemento_en_pos(*juego, pos_arriba)==LETRA_ESCALERA){
        pos_arriba.fil--;
    }
    juego->homero.posicion=pos_arriba;
}

/*
 * Pre condiciones: El juego debe estar correctamente inicializado y la acción ser válida.
 * Pos condiciones: Dependiendo del bloque, interactúa, rompe o resta resistencia a él.
*/
void chocar_con_bloque(juego_t *juego, int indice_bloque, char accion){
    int costo_romper=juego->bloques[indice_bloque].resistencia;
    if(juego->bloques[indice_bloque].tipo==LETRA_VIGA)
        printf("No es posible romper este bloque.\n");
    else if(juego->bloques[indice_bloque].tipo==LETRA_ESCALERA && accion!=MOVERSE_ABAJO){
        interactuar_escaleras(juego, indice_bloque);
        juego->movimientos_restantes-=COSTO_MOVIMIENTO;
        interactuar_elementos(juego);
    } else if(costo_romper>COSTO_MOVIMIENTO){
        juego->bloques[indice_bloque].resistencia-=COSTO_MOVIMIENTO;
        juego->movimientos_restantes-=COSTO_MOVIMIENTO;
    } else if(costo_romper==COSTO_MOVIMIENTO){
        romper_bloque(juego, indice_bloque);
        juego->movimientos_restantes-=COSTO_MOVIMIENTO;
    }
}

/*
 * Pre condiciones: El juego debe estar correctamente inicializado y la acción ser un movimiento válido.
 * Pos condiciones: Dependiendo de si hay un bloque en la dirección o no, choca con este o modifica la posición del personaje.
*/
void ejecutar_movimiento(juego_t *juego, char accion){
    int indice_bloque=-1;
    if(hay_bloque_en_direccion(*juego, accion, &indice_bloque)){
        chocar_con_bloque(juego, indice_bloque, accion);
    } else{
        mover_homero(juego, accion);
        interactuar_elementos(juego);
    }
}

/*
 * Pre condiciones: -
 * Pos condiciones: Valida si la acción es USAR_ESCALERA o USAR_SOPLETE.
*/
bool es_usar_herramienta(char accion){
    bool es_usar_herramienta=false;
    if(accion==USAR_ESCALERA || accion==USAR_SOPLETE)
        es_usar_herramienta=true;
    return es_usar_herramienta;
}

/*
 * Pre condiciones: El juego debe estar correctamente inicializado.
 * Pos condiciones: Valida si es posible usar la respectiva herramienta pasada por parámetro o no.
*/
bool esta_disponible_herramienta(juego_t juego, char accion){
    bool esta_disponible=false;
    int i=0;
    if(accion==USAR_ESCALERA && juego.homero.cantidad_madera>=COSTO_ESCALERA 
        && !hay_bloque_en_direccion(juego, accion, &i) && es_movimiento_posible(juego, accion))
        esta_disponible=true;
    if(accion==USAR_SOPLETE && juego.homero.cantidad_sopletes>=COSTO_SOPLETE)
        esta_disponible=true;
    return esta_disponible;
}

/*
 * Pre condiciones: El juego debe estar correctamente inicializado y el personaje tener maderas suficientes.
 * Pos condiciones: Agrega una escalera al final del vector bloques, aumenta el tope del mismo y resta COSTO_ESCALERA a la madera del personaje.
*/
void crear_escalera(juego_t *juego){
    juego->bloques[juego->tope_bloques].posicion=juego->homero.posicion;
    juego->bloques[juego->tope_bloques].tipo=LETRA_ESCALERA;
    juego->bloques[juego->tope_bloques].resistencia=RESIST_ESCALERA;
    juego->tope_bloques++;
    juego->homero.cantidad_madera-=COSTO_ESCALERA;
}

/*
 * Pre condiciones: Las posicione deben ser válidas.
 * Pos condiciones: Devuelve la distancia manhattan entre ambas posiciones recibidas por parámetro.
*/
int devuelve_dist_manhattan(coordenada_t pos1, coordenada_t pos2){
    int distancia=(abs(pos1.fil-pos2.fil)+abs(pos1.col-pos2.col));
    return distancia;
}

/*
 * Pre condiciones: El juego debe estar correctamente inicializado y el personaje tener un soplete como mínimo.
 * Pos condiciones: Elimina todos los bloques que no sean vigas y se encuentren dentro de RADIO_SOPLETE del vector bloques.
*/
void usar_soplete(juego_t *juego){
    int i=0;
    while(i<juego->tope_bloques){
        if(devuelve_dist_manhattan(juego->homero.posicion, juego->bloques[i].posicion)<=RADIO_SOPLETE && juego->bloques[i].tipo!=LETRA_VIGA){
            if(juego->bloques[i].tipo==LETRA_TELARANIA){
                while(devuelve_dist_manhattan(juego->homero.posicion, juego->bloques[i].posicion)<=RADIO_SOPLETE){
                    romper_bloque(juego, i);
                }
                i++;
            } else{
                romper_bloque(juego, i);
            }
        } else{
            i++;
        }
    }
    juego->homero.cantidad_sopletes-=COSTO_SOPLETE;
}

/*
 * Pre condiciones: El juego debe estar correctamente inicializado y esta_disponible_herramienta ser true.
 * Pos condiciones: Dependiendo de si la acción es USAR_ESCALERA o USAR_SOPLETE, llama a las funciones crear_escalera o usar_soplete.
*/
void usar_herramienta(juego_t *juego, char accion){
    if(accion==USAR_ESCALERA){
        crear_escalera(juego);
        juego->homero.posicion.fil--;
        interactuar_elementos(juego);
    }
    else if(accion==USAR_SOPLETE)
        usar_soplete(juego);
}

/*
 * Pre condiciones: El juego debe estar inicializado previamente con `inicializar_juego` y la acción
 * debe ser válida.
 * Post condiciones: Realizará la acción recibida por parámetro actualizando el juego.
 */
void realizar_jugada(juego_t *juego, char movimiento){
    char accion=movimiento;
    if(es_movimiento(accion)){
        if(es_movimiento_posible(*juego, accion)){
            ejecutar_movimiento(juego, accion);
        } else{
            printf("Ese movimiento no es posible.\n");
        }
    } else if(es_usar_herramienta(accion)){
        if(esta_disponible_herramienta(*juego, accion)){
            usar_herramienta(juego, accion);
        } else{
            printf("No es posible usar esa herramienta ahora mismo.\n");
        }
    }
    caer_por_gravedad(juego);
    if(juego->movimientos_restantes<CANT_MOV_NULOS)
        juego->movimientos_restantes=CANT_MOV_NULOS;
}

/*
 * Pre condiciones: El juego deberá estar inicializado previamente con `inicializar_juego `
 * Post condiciones: Devuelve:
 * --> 1 si es ganado
 * --> -1 si es perdido
 * --> 0 si se sigue jugando
 * El juego se dará por ganado cuando Homero recolecta su receta secreta y llega a Moe.
 * Se dará por perdido si se le terminan los movimientos antes de recolectar la receta y llegar a Moe.
 */
int estado_juego(juego_t juego){
    int estado_juego=SIGUE_JUGANDO;
    if(juego.homero.recolecto_receta && son_pos_iguales(juego.homero.posicion, juego.posicion_moe))
        estado_juego=JUEGO_GANADO;
    else if(juego.movimientos_restantes==CANT_MOV_NULOS)
        estado_juego=JUEGO_PERDIDO;
    return estado_juego;
}