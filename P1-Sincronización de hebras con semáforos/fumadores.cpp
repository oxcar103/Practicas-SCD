#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>          // incluye ”time(....)”
#include <unistd.h>        // incluye ”usleep(...)”
#include <stdlib.h>        // incluye ”rand(...)” y ”srand”

using namespace std ;

sem_t tabaco;       // Semáforo para indicar si hay tabaco
sem_t papel;        // Semáforo para indicar si hay papel
sem_t cerillas;     // Semáforo para indicar si hay cerillas
sem_t mostrador;    // Semáforo para indicar si el mostrador está vacío

// Función que simula la acción de fumar
// como un retardo aleatorio de la hebra entre 0'1 y 2 segundos.
void fumar(){
    // Calculamos un número aleatorio de milisegundos (entre 1/10 y 2 segundos)
    const unsigned miliseg = 100U + (rand() % 1900U) ;
    
    usleep(1000U*miliseg); // retraso bloqueado durante miliseg milisegundos
}

// Función para simular al fumador sin tabaco
void * sin_tabaco(void *){
    while (true){
        // Nos aseguramos de que hay tabaco
        sem_wait(&tabaco);
        // Indicamos que el mostrador está vacío
        sem_post(&mostrador);

        // Mostramos por pantalla que se ha recogido tabaco y va a fumar
        cout << "- Recogí tabaco, entro a fumar.\n";

        fumar();

        // Mostramos por pantalla que ha terminado de fumar y espera recoger más tabaco
        cout << "- Terminé de fumar, me falta tabaco.\n";
    }

    return NULL;
}

// Función para simular al fumador sin papel
void * sin_papel(void *){
    while (true){
        // Nos aseguramos de que hay papel
        sem_wait(&papel);
        // Indicamos que el mostrador está vacío
        sem_post(&mostrador);

        // Mostramos por pantalla que se ha recogido papel y va a fumar
        cout << "- Recogí papel, entro a fumar.\n";

        fumar();

        // Mostramos por pantalla que ha terminado de fumar y espera recoger más papel
        cout << "- Terminé de fumar, me falta papel.\n";
    }

    return NULL;
}

// Función para simular al fumador sin cerillas
void * sin_cerillas(void *){
    while (true){
        // Nos aseguramos de que hay cerillas
        sem_wait(&cerillas);
        // Indicamos que el mostrador está vacío
        sem_post(&mostrador);

        // Mostramos por pantalla que se ha recogido cerillas y va a fumar
        cout << "- Recogí cerillas, entro a fumar.\n";

        fumar();

        // Mostramos por pantalla que ha terminado de fumar y espera recoger más cerillas
        cout << "- Terminé de fumar, me faltan cerillas.\n";
    }

    return NULL;
}

// Función para simular al estanquero
void * estanquero(void *){
    while (true){
        // Nos aseguramos de que el mostrador está vacío
        sem_wait(&mostrador);

        // Indicamos qué pone el estanquero(tanto por pantalla como mediante los semáforos)
        cout << "El estanquero pone ";

        switch (rand()%3) {
            case 0: cout << "tabaco\n";
                sem_post(&tabaco);
                break;
            case 1: cout << "papel\n";
                sem_post(&papel);
                break;
            case 2: cout << "cerillas\n";
                sem_post(&cerillas);
                break;
        }
    }

    return NULL;
}

int main( int argc, char * argv[] ){
    pthread_t t_fumador_sin_tabaco, t_fumador_sin_papel, t_fumador_sin_cerillas, t_estanquero;

    srand(time(NULL)); // inicializa la semilla aleatoria

    // Inicializamos los semáforos
    sem_init(&mostrador, 0, 1);
    sem_init(&papel, 0, 0);
    sem_init(&cerillas, 0, 0);
    sem_init(&tabaco, 0, 0);

    // Creamos las hebras
    pthread_create(&t_fumador_sin_tabaco, NULL, sin_tabaco, NULL);
    pthread_create(&t_fumador_sin_papel, NULL, sin_papel, NULL);
    pthread_create(&t_fumador_sin_cerillas, NULL, sin_cerillas, NULL);
    pthread_create(&t_estanquero, NULL, estanquero, NULL);

    // Esperamos a que acaben    
    pthread_join(t_fumador_sin_tabaco, NULL);
    pthread_join(t_fumador_sin_papel, NULL);
    pthread_join(t_fumador_sin_cerillas, NULL);
    pthread_join(t_estanquero, NULL);

    // Destruimos los semáforos
    sem_destroy(&mostrador);
    sem_destroy(&papel);
    sem_destroy(&cerillas);
    sem_destroy(&tabaco);

    pthread_exit(NULL);

    return 0;
}

