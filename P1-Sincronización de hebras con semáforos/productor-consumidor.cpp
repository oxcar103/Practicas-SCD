#include <iostream>
#include <pthread.h>
#include <semaphore.h>

using namespace std ;

const int NUM_ITEMS = 103;
const int TAM_VEC = 13;
int buffer[TAM_VEC];
int primera_ocupada, primera_libre = primera_ocupada = 0;

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

        //Falta insertar dato en buffer
    }

    return NULL;
}

void* consumidor(void*){
    for(unsigned int i = 0; i < NUM_ITEMS; i++){
        int dato;

        //Falta coger dato de buffer

        consumir_dato(dato);
    }

}

int main(){
    pthread_t t_productor, t_consumidor;

    // Inicializamos los semáforos

    // Creamos las hebras    
    pthread_create(&t_productor, NULL, productor, NULL);
    pthread_create(&t_consumidor, NULL, consumidor, NULL);

    // Esperamos a que acaben
    pthread_join(t_productor,NULL);
    pthread_join(t_consumidor,NULL);

    cout << "Fin.\n";

    // Destruimos los semáforos

    pthread_exit(NULL);
}

