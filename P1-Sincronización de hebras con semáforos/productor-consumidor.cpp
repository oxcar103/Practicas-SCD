#include <iostream>
#include <pthread.h>
#include <semaphore.h>

using namespace std ;

const int NUM_ITEMS = 103;
const int TAM_VEC = 13;
int buffer[TAM_VEC];
int primera_libre = primera_ocupada = 0;
sem_t acceso_a_buffer;
//sem_t buffer_con_elementos;
//sem_t buffer_con_espacio;

int producir_dato(){
    static int contador = 1;
    return contador++;
}

void consumir_dato(int dato){
    cout << "Dato recibido: " << dato << "\n";
}

void* productor(void*){
    for(unsigned int i = 0; i < NUM_ITEMS; i++){
        int dato = producir_dato();

        sem_wait(&acceso_a_buffer);
        //Falta insertar dato en buffer
        sem_post(&acceso_a_buffer);
    }

    return NULL;
}

void* consumidor(void*){
    for(unsigned int i = 0; i < NUM_ITEMS; i++){
        int dato;

        sem_wait(&acceso_a_buffer);
        //Falta coger dato de buffer
        sem_post(&acceso_a_buffer);

        consumir_dato(dato);
    }

}

int main(){
    pthread_t t_productor, t_consumidor;

    // Inicializamos los semáforos
    sem_init(&accesso_a_buffer, 0, 1);
//    sem_init(&buffer_con_elementos, 0, 0);
//    sem_init(&buffer_con_espacio, 0, 1);

    // Creamos las hebras    
    pthread_create(&t_productor, NULL, productor, NULL);
    pthread_create(&t_consumidor, NULL, consumidor, NULL);

    // Esperamos a que acaben
    pthread_join(hebra1,NULL);
    pthread_join(hebra2,NULL);

    cout << "Fin.\n";

    // Destruimos los semáforos
    sem_destroy(&accesso_a_buffer);
//    sem_destroy(&buffer_con_elementos);
//    sem_destroy(&buffer_con_espacio);

    pthread_exit(NULL);
}

