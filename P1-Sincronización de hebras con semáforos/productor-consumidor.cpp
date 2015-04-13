#include <iostream>
#include <pthread.h>
#include <semaphore.h>

using namespace std;

const int NUM_ITEMS = 103;
const int TAM_VEC = 13;
int buffer[TAM_VEC];
int primera_ocupada, primera_libre = primera_ocupada = 0;
sem_t acceso_a_buffer;
sem_t buffer_con_elementos;
sem_t buffer_con_espacio;

int producir_dato(){
    static int contador = 1;
    return contador++;
}

void consumir_dato(int dato){
    cout << "Dato recibido: " << dato << "\n";
}

void* productor(void*){
    for(int i = 0; i < NUM_ITEMS; i++){
        int dato = producir_dato();

        // Insertamos dato en buffer
        sem_wait(&buffer_con_espacio);
        sem_wait(&acceso_a_buffer);

        buffer[primera_libre] = dato;
        primera_libre = (primera_libre+1) % TAM_VEC;

        sem_post(&acceso_a_buffer);
        sem_post(&buffer_con_elementos);
    }

    return NULL;
}

void* consumidor(void*){
    for(int i = 0; i < NUM_ITEMS; i++){
        int dato;

        // Cogemos dato de buffer
        sem_wait(&buffer_con_elementos);
        sem_wait(&acceso_a_buffer);

        dato = buffer[primera_ocupada];
        primera_ocupada = (primera_ocupada+1) % TAM_VEC;

        sem_post(&acceso_a_buffer);
        sem_post(&buffer_con_espacio);


        consumir_dato(dato);
    }

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

