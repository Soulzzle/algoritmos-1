# Algoritmos y Programación I - Trabajos Prácticos (2C2025)

Este repositorio contiene las entregas de los Trabajos Prácticos (TP0, TP1 y TP2) desarrollados para la materia **Algoritmos y Programación I** durante el **Segundo Cuatrimestre de 2025 (2C2025)**. Todas las prácticas están ambientadas en el universo de **Los Simpson**.

---

## 📋 Índice
1. [TP0: La Llamarada Moe 🍸](#tp0-la-llamarada-moe-)
2. [TP1: Simpsoncraft 🪜](#tp1-simpsoncraft-)
3. [TP2: La Lista Negra 🚫](#tp2-la-lista-negra-)
4. [🛠️ Compilación y Ejecución](#️-compilación-y-ejecución)

---

## 🍸 TP0: La Llamarada Moe

## Este es el único TP del cual no cuento con el enunciado en PDF.

### 📖 Descripción
Un cuestionario interactivo en consola donde el usuario responde las preguntas de Moe Szyslak antes de pedir un trago. El programa evalúa la edad del cliente y calcula su estado de ebriedad según las decisiones tomadas durante el pedido.

### 🎮 Mecánica de Juego
1. **Verificación de Edad**: 
   - El cliente ingresa su fecha de nacimiento en formato `yyyy.mm`.
   - Se calcula la edad actual respecto al año y mes lectivo (2025).
   - Si es menor de 18 años, el cliente es **REBOTADO** de inmediato.
2. **Elección de Hielo**:
   - Se consulta si desea hielo (`S` / `N`).
3. **Selección de Trago**:
   - **Whiskey** (`W`): 40 puntos base.
   - **GinTonic** (`G`): 30 puntos base.
   - **Fernet** (`F`): 20 puntos base.
   - **Campari** (`C`): 10 puntos base.
4. **Cantidad de Vasos**:
   - Se ingresa la cantidad consumida (entre 0 y 10 vasos).

### 📐 Sistema de Puntuación y Resultados
- **Puntos por trago**: `Puntos Base - Puntos por Hielo`
  - Con hielo (`S`): +5 puntos.
  - Sin hielo (`N`): -100 puntos.
- **Puntuación Total**: `Puntos por trago * Cantidad de vasos`

| Puntaje Total | Estado Resultante |
| :--- | :--- |
| **0 a 100** | 🟢 **SOBRIO** |
| **101 a 200** | ⚪ **ALEGRE** |
| **201 a 300** | 🟡 **BORRACHO** |
| **Fuera de rango (<0 o >300)** | 🔴 **HOSPITAL** |

---

## 🪜 TP1: Simpsoncraft

### 📖 Descripción
Un juego de estrategia y exploración 2D por turnos inspirado en la estética de Minecraft y Los Simpson. El jugador encarna a **Homero Simpson** (`H`) dentro de un mapa de cuadrícula (30 filas x 15 columnas), con el objetivo de encontrar la **Receta Secreta** (`R`) de la Llamarada Moe y entregársela a **Moe** (`M`) antes de agotar los movimientos disponibles.

### 🎮 Mecánica de Juego
- **Tablero**: Cuadrícula de 30x15 casilleros con bloques, herramientas y obstáculos.
- **Objetivos**:
  1. Recolectar la Receta Secreta (`R`).
  2. Llegar a la posición de Moe (`M`).
- **Recursos e Ítems**:
  - **Madera**: Se recolecta del mapa para construir escaleras.
  - **Soplete**: Permite fundir u omitir obstáculos según la resistencia del bloque.
  - **Escaleras**: Permiten ascender y descender en los niveles del terreno.
- **Condición de Victoria**: Recolectar la receta `R` y alcanzar a Moe `M`.
- **Condición de Derrota**: Quedarse sin movimientos restantes antes de cumplir ambos objetivos.

### 🎮 Controles
| Tecla | Acción |
| :---: | :--- |
| `D` | Moverse a la **Derecha** |
| `A` | Moverse a la **Izquierda** |
| `S` | Moverse hacia **Abajo** |
| `E` | Usar / Construir **Escalera** |
| `O` | Usar **Soplete** |

---

## 🚫 TP2: La Lista Negra

### 📖 Descripción
Un juego de simulación de seguridad y control de acceso en la puerta del Bar de Moe. El jugador asume el rol del guardia de entrada y debe decidir a quién permitir el ingreso y a quién rechazárselo, memorizando una **Lista Negra** de personas indeseadas. El sistema procesa archivos CSV y mantiene un **Ranking** de partidas ordenado automáticamente.

### 🎮 Mecánica de Juego
1. **Fase de Memorización**: Al iniciar la partida, el juego carga un archivo con la Lista Negra de personas indeseadas y la imprime en pantalla para que el jugador memorice los nombres.
2. **Entrada de Clientes**: Los clientes se presentan de a uno en la puerta (generados mediante el TDA Generador).
3. **Decisión del Jugador**: 
   - `S`: Permitir el paso al bar.
   - `N`: Rechazar el ingreso.
4. **Fin de Juego (Game Over)**: La partida finaliza inmediatamente si el jugador le permite el ingreso (`S`) a una persona que figura en la Lista Negra.

### 💯 Sistema de Puntuación
- **Permitir el paso a una persona autorizada** (`S` a persona NO en lista): **+1 punto**
- **Rechazar el paso a una persona autorizada** (`N` a persona NO en lista): **-1 punto**
- **Rechazar el paso a una persona indeseada** (`N` a persona SÍ en lista): **+3 puntos bonus**
- **Permitir el paso a una persona indeseada** (`S` a persona SÍ en lista): **PERDER (Game Over)**

### 🏆 Gestión de Rankings y Archivos
- El ejecutable recibe las rutas de la lista negra y del ranking por argumentos de línea de comandos.
- Al perder, calcula el puntaje y el puesto del jugador, insertando la partida de forma ordenada según los siguientes criterios:
  1. **Puntaje** (de mayor a menor).
  2. **Cantidad de personas en la Lista Negra** (de mayor a menor).
  3. **Nombre del Jugador** (alfabéticamente).

---

## 🛠️ Compilación y Ejecución

**Puede usarse el ejecutable que ya viene creado en cada TP**

### Prerrequisitos
Tener instalado un compilador de C como `gcc`.

### TP0: La Llamarada Moe
```bash
cd TP0
gcc -std=c99 -Wall -Wconversion -Werror -o llamarada llamarada.c
./llamarada
```

### TP1: Simpsoncraft
```bash
cd TP1
gcc -std=c99 -Wall -Wconversion -Werror juego.c simpsoncraft.c -o juego
./juego
```

### TP2: La Lista Negra
```bash
cd TP2
gcc -std=c99 -Wall -Wconversion -Werror lista_negra.c generador.o -o lista_negra
./lista_negra lista_negrita.csv ranking.csv
```
