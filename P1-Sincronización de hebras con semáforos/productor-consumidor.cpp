#include <iostream>
#include <pthread.h>
#include <semaphore.h>

using namespace std;

const int NUM_ITEMS = 103;  // Número de elementos que se escribirán/leerán
const int TAM_VEC = 13;     // Tamaño del buffer
int buffer[TAM_VEC];
int primera_ocupada, primera_libre = primera_ocupada = 0;   // Para indicar las posiciones donde leer y escribir
sem_t acceso_a_buffer;       // Semáforo para asegurar que no se modifican varias posiciones del buffer a la vez
sem_t buffer_con_elementos;  // Semáforo para asegurar que no se lee cuando no hay elementos
sem_t buffer_con_espacio;    // Semáforo para asegurar que no se escribe cuando el buffer está lleno

// Función para producir los elementos
int producir_dato(){
    static int contador = 1;
    return contador++;
}

// Función para usar los elementos
void consumir_dato(int dato){
    cout << "Dato recibido: " << dato << "\n";
}

// Función para producir y escribir los elementos
void* productor(void*){
    for(int i = 0; i < NUM_ITEMS; i++){
        int dato = producir_dato();

        // Nos aseguramos de que podemos escribir
        sem_wait(&buffer_con_espacio);
        // Nos aseguramos de que sólo nosotros accedemos al buffer
        sem_wait(&acceso_a_buffer);

        // Insertamos el dato en el buffer y ajustamos la variable indicadora de posición para escribir
        buffer[primera_libre] = dato;
        primera_libre = (primera_libre+1) % TAM_VEC;

        // Permitimos a otro acceder al buffer
        sem_post(&acceso_a_buffer);
        // Indicamos que hay un nuevo elemento
        sem_post(&buffer_con_elementos);
    }

    return NULL;
}

// Función para leer y usar los elementos
void* consumidor(void*){
    for(int i = 0; i < NUM_ITEMS; i++){
        int dato;

        // Nos aseguramos de que podemos leer
        sem_wait(&buffer_con_elementos);
        // Nos aseguramos de que sólo nosotros accedemos al buffer
        sem_wait(&acceso_a_buffer);

        // Cogemos el dato del buffer y ajustamos la variable indicadora de posición para leer
        dato = buffer[primera_ocupada];
        primera_ocupada = (primera_ocupada+1) % TAM_VEC;

        // Permitimos a otro acceder al buffer
        sem_post(&acceso_a_buffer);
        // Indicamos que se ha leido un elemento
        sem_post(&buffer_con_espacio);

        // Usamos el dato
        consumir_dato(dato);
    }

    return NULL;
}

int main(){
    pthread_t t_productor, t_consumidor;

    // Inicializamos los semáforos
    sem_init(&acceso_a_buffer, 0, 1);
    sem_init(&buffer_con_elementos, 0, 0);
    sem_init(&buffer_con_espacio, 0, TAM_VEC);

    // Creamos las hebras    
    pthread_create(&t_productor, NULL, productor, NULL);
    pthread_create(&t_consumidor, NULL, consumidor, NULL);

    // Esperamos a que acaben
    pthread_join(t_productor,NULL);
    pthread_join(t_consumidor,NULL);

    cout << "Fin.\n";

    // Destruimos los semáforos
    sem_destroy(&acceso_a_buffer);
    sem_destroy(&buffer_con_elementos);
    sem_destroy(&buffer_con_espacio);

    pthread_exit(NULL);
}

