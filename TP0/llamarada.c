#include <stdio.h>
#include <stdbool.h>

const int ANIO_ACTUAL=2025;
const int ANIO_MINIMO=1000;
const int MES_ACTUAL=9;
const int MES_MINIMO=1;
const int MES_MAXIMO=12;
const int EDAD_MINIMA=18;
const char RESPUESTA_AFIRMATIVA='S';
const char RESPUESTA_NEGATIVA='N';
const int VASOS_MINIMOS=0;
const int VASOS_MAXIMOS=10;
const int PUNTOS_CON_HIELO=5;
const int PUNTOS_SIN_HIELO=-100;
const int PUNTOS_WHISKEY=40;
const int PUNTOS_GINTONIC=30;
const int PUNTOS_FERNET=20;
const int PUNTOS_CAMPARI=10;
const int MINIMO_SOBRIO=0;
const int MAXIMO_SOBRIO=100;
const int MINIMO_ALEGRE=101;
const int MAXIMO_ALEGRE=200;
const int MINIMO_BORRACHO=201;
const int MAXIMO_BORRACHO=300;
#define WHISKEY 'W'
#define GINTONIC 'G'
#define FERNET 'F'
#define CAMPARI 'C'

/*
PRE-CONDICIONES: -
POST-CONDICIONES: Da... la... bienvenida
*/ 
void bienvenida(){
    printf("\033[1;96m\n¡Bienvenid@ al bar de Moe!\nAntes de servirte, me gustaría hacerte unas preguntas...\033[0m\n\n");
}

/*
PRE-CONDICIONES: -
POST-CONDICIONES: Pregunta al cliente la fecha de nacimiento y la guarda en la variable pasada por referecia.
*/
void preguntarFechaNacimiento(double* fecha, int* anio, int* mes){
    printf("\033[1;34m¿Cuál es tu fecha de nacimiento? \033[0;34m(En el formato yyyy.mm): \033[0m");
    scanf("%lf", fecha);
    *anio=(int)*fecha;
    *mes=(int)(*fecha*100)-(*anio*100);
    while ((*anio<ANIO_MINIMO || *anio>ANIO_ACTUAL) || (*mes<MES_MINIMO || *mes>MES_MAXIMO)){
        printf("\033[1;34mFecha inválida. ¿Cuál es tu fecha de nacimiento? \033[0;34m(En el formato yyyy.mm): \033[0m");
        scanf("%lf", fecha);
        *anio=(int)*fecha;
        *mes=(int)(*fecha*100)-(*anio*100);
    }
}

/*
PRE-CONDICIONES: -
POST-CONDICIONES: Devuelve la edad del cliente.
*/
int devuelveEdad(int anio, int mes){
    int edad=0;
    if (mes<=MES_ACTUAL){
        edad=ANIO_ACTUAL-anio;
    } else{
        edad=(ANIO_ACTUAL-anio)-1;
    }
    return edad;
}

/*
PRE-CONDICIONES: -
POST-CONDICIONES: Valida si el cliente tiene la edad mínima necesaria.
*/
bool esClienteValido(){
    double fecha_nacimiento=0.00;
    int anio=0;
    int mes=0;
    preguntarFechaNacimiento(&fecha_nacimiento, &anio, &mes);
    int edad_cliente=devuelveEdad(anio, mes);
    return edad_cliente>=EDAD_MINIMA;
}

/*
PRE-CONDICIONES: -
POST-CONDICIONES: Pregunta al cliente si quiere hielo y lo guarda en la variable pasada por referencia.
*/
void preguntarQuiereHielo(char* respuesta){
    printf("\033[1;34m¿Quiere hielo? \033[0;34m('S'= Sí 'N'= No): \033[0m");
    scanf(" %c", respuesta);
    while (*respuesta!=RESPUESTA_AFIRMATIVA && *respuesta!=RESPUESTA_NEGATIVA){
        printf("\033[1;34m¿...? Es sí o no... ¿Quiere hielo? \033[0;34m('S'= Sí 'N'= No): \033[0m");
        scanf(" %c", respuesta);
    }
}

/*
PRE-CONDICIONES: -
POST-CONDICIONES: Valida si el cliente quiere hielo.
*/
bool quiereHielo(){
    char respuesta;
    preguntarQuiereHielo(&respuesta);
    return respuesta==RESPUESTA_AFIRMATIVA;
}

/*
PRE-CONDICIONES: -
POST-CONDICIONES: Pregunta al cliente qué trago quiere y lo guarda en la variable pasada por referencia.
*/
void preguntarTrago(char* trago){
    printf("\033[1;34m¿Qué trago quiere? \033[0;34m('W'=Whiskey 'G'=GinTonic 'F'=Fernet 'C'=Campari): \033[0m");
    scanf(" %c", trago);
    while (*trago!=WHISKEY && *trago!=GINTONIC && *trago!=FERNET && *trago!=CAMPARI){
        printf("\033[1;34mSolo te puedo ofrecer estos: \033[0;34m('W'=Whiskey 'G'=GinTonic 'F'=Fernet 'C'=Campari): \033[0m");
        scanf(" %c", trago);
    }
}

/*
PRE-CONDICIONES: -
POST-CONDICIONES: Devuelve el trago preferido por el cliente.
*/
char devuelveTrago(){
    char trago;
    preguntarTrago(&trago);
    return trago;
}

/*
PRE-CONDICIONES: -
POST-CONDICIONES: Pregunta al cliente cuántos vasos quiere y lo guarda en la variable pasada por referencia.
*/
void preguntarCantidadVasos(int* cantidad_vasos){
    printf("\033[1;34m¿Cuántos vasos suele consumir? \033[0;34m(No más de 10 eh): \033[0m");
    scanf(" %i", cantidad_vasos);
    while (*cantidad_vasos<VASOS_MINIMOS || *cantidad_vasos>VASOS_MAXIMOS){
        printf("\033[1;34mEstás tirando cualquiera... Decime cuántos vasos cosumís \033[0;34m(desde 0 hasta 10): \033[0m");
        scanf(" %i", cantidad_vasos);
    }
}

/*
PRE-CONDICIONES: -
POST-CONDICIONES: Devuelve la cantidad de vasos preferida por el cliente.
*/
int devuelveVasos(){
    int cantidad_vasos;
    preguntarCantidadVasos(&cantidad_vasos);
    return cantidad_vasos;
}

/*
PRE-CONDICIONES: -
POST-CONDICIONES: Calcula y devuelve los puntos totales del cliente.
*/
int devuelvePuntos(bool quiere_hielo, char trago, int cantidad_vasos){
    int puntos_hielo=0;
    int puntos_trago=0;
    int puntos_totales=0;
    if (quiere_hielo){
        puntos_hielo=PUNTOS_CON_HIELO;
    } else{
        puntos_hielo=PUNTOS_SIN_HIELO;
    }
    switch(trago){
        case WHISKEY:
            puntos_trago=PUNTOS_WHISKEY-puntos_hielo;
            break;
        case GINTONIC:
            puntos_trago=PUNTOS_GINTONIC-puntos_hielo;
            break;
        case FERNET:
            puntos_trago=PUNTOS_FERNET-puntos_hielo;
            break;
        case CAMPARI:
            puntos_trago=PUNTOS_CAMPARI-puntos_hielo;
            break;
    }
    puntos_totales=puntos_trago*cantidad_vasos;
    return puntos_totales;
}

/*
PRE-CONDICIONES: -
POST-CONDICIONES: Muestra por pantalla el estado del cliente en base a sus puntos totales.
*/
void mostrarResultadoFinal(int puntos_totales){
    if (puntos_totales>=MINIMO_SOBRIO && puntos_totales<=MAXIMO_SOBRIO){
        printf("\033[1;96mPor tus decisiones, tu estado es \033[1;32m-SOBRIO-\033[0m\n");
    } else if (puntos_totales>=MINIMO_ALEGRE && puntos_totales<=MAXIMO_ALEGRE){
        printf("\033[1;96mPor tus decisiones, tu estado es \033[1;97m-ALEGRE-\033[0m\n");
    } else if (puntos_totales>=MINIMO_BORRACHO && puntos_totales<=MAXIMO_BORRACHO){
        printf("\033[1;96mPor tus decisiones, tu estado es \033[1;33m-BORRACHO-\033[0m\n");
    } else{
        printf("\033[1;33mPor tus decisiones, tu estado es \033[4;1;31m-HOSPITAL-\033[0m\n");
    }
}

int main(){
    bienvenida();
    if (esClienteValido()){
        bool quiere_hielo=quiereHielo();
        char trago=devuelveTrago();
        int cantidad_vasos=devuelveVasos();
        int puntos_totales=devuelvePuntos(quiere_hielo, trago, cantidad_vasos);
        mostrarResultadoFinal(puntos_totales);
    } else{
        printf("\033[1;33mSos menor de edad, volá de acá. \033[1;31m-REBOTADO-\033[0m\n");
    }

    return 0;
}