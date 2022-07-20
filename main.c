#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// Número máximo de reinas con las que se va a jugar.
unsigned N_MAX_REINAS = 25;

// Contendrá el array de reinas.
long long *reinas;

// Libera la memoria dinámica adquirida cuando el programa se cierra
void liberar() {
    if(reinas != NULL)
        free(reinas);
}

// Comprueba que la reina fila indicada no esté amenazada por otra en la misma
// vertical o en cualquiera de sus diagonales.
// Devuelve true y está amenazada, false en caso contrario.
bool comprobar_amenaza(long long fila_reina) {
    for(long long i=0; i<fila_reina; ++i) {
        if(reinas[fila_reina] == reinas[i]
        || reinas[fila_reina]-fila_reina == reinas[i]-i
        || reinas[fila_reina]+fila_reina == reinas[i]+i)
        return true;
    }
    return false;
}

// Imprime el tablero actual.
// Marca las casillas con reina con XX, si no la hay
// pone espacios.
void imprimir(unsigned dimension) {
    puts("");
    for(unsigned i=0; i<dimension; ++i) {
        for(unsigned j=0; j<dimension; ++j)
            printf("---");
        puts("-");
        putchar('|');
        for(unsigned j=0; j<dimension; ++j) {
            printf("%s|", j==reinas[i]? "XX" : "  ");
        }
        puts("");
    }
    for(unsigned j=0; j<dimension; ++j)
        printf("---");
    puts("-\n");
}

int main() {
    // No hay un tablero en sí sino un array de números
    // cada uno representa una reina en una fila. Dichos números
    // indican la columna en que se ha posicionado la reina.
    reinas = malloc((N_MAX_REINAS+1) * sizeof(long long));
    
    atexit(liberar);
    
    // Comprueba tantos tableros de dimensión x dimensión que se indiquen.
    for(unsigned dimension=1; dimension<=N_MAX_REINAS; ++dimension) {

        // Número de soluciones encontradas para el tablero actual.
        unsigned soluciones = 0;

        // Índice o fila de la reina a tratar.
        long long i = 0;
        
        // Se inicializa con la reina de la fila 0 en casilla 0.
        reinas[0] = 0;
        
        // Bucle principal del juego. En él se irán colocando todas las reinas
        // y calculará las posiciones correctas para que las reinas no se ataquen.
        // Se usa el backtracking para posicionar las reinas.
        // El bucle se detendrá cuándo la reina de la fila 0 salga del tablero.
        while(true) {
            if(reinas[i] >= dimension) {
                // En caso de que la reina actual se haya posicionado fuera del tablero.
                if(i == 0)
                    // En caso de ser la de la fila 0 significa que el tablero se revisó
                    // completamente. Se detiene el bucle del juego para pasar al
                    // sigiuente tablero.
                    break;
                // No es la primera reina.
                else
                    // En este caso se va a trabajar con la reina de la fila inmediatamente
                    // superior y se la hace avanzar una columna para recalcular a partir
                    // de esa posición.
                    reinas[--i]++;
            } else {
                // La reina está dentro de los límites del tablero, por tanto se mira
                // si en su posición está siendo amenazada.
                if(!comprobar_amenaza(i)) {
                    // No está amenazada. Se controla si esa reina es la de la última fila.
                    if(i < dimension-1)
                        // No lo és, se prepara la reina de la siguiente línea para trabajar
                        // con ella a partir de ahora.
                        reinas[++i] = -1;
                    else {
                        // Es la reina de la última fila, por tanto hemos encontrado una jugada
                        // tal que todas las reinas están posicionadas y ninguna está amenazada.
                        // Imprimir el tablero (la salida puede ser muy larga y tal vez pueda ser
                        // interesante el comentar la siguiente línea)...
                        imprimir(dimension);
                        // ... y actualizar el contador de soluciones
                        soluciones++;
                    }
                }
                // En todo caso, tanto si la reina está amenazada como si no lo está, avanzará una
                // columna para seguir comprobando el resto de posibilidades del tablero.
                reinas[i]++;
            }
        }
        // Fin del cálculo de todas las posibilidades del tablero. Se muestra el resultado.
        printf("f(%d) = %d\n\n", dimension, soluciones);
    }
}
