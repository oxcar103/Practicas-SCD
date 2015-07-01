#include "mpi.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>

#define COGER 0                 // Tag de Coger
#define SOLTAR 1                // Tag de Soltar
#define SENTARSE 2              // Tag de Sentarse
#define LEVANTARSE 3            // Tag de Levantarse

#define NUM_FILO 5              // Número de Filósofos
#define CAMARERO 2*NUM_FILO     // El último proceso es el Camarero

using namespace std;

void Filosofo(int id, int nprocesos){
    int izq = (id+1)%nprocesos;
    int der = (id+(nprocesos-1))%nprocesos;   

    while(true){
        // Solicita sentarse
        cout << "Filósofo " << id << " solicita sentarse\n";

        MPI_Send(NULL, 0, MPI_INT, CAMARERO, SENTARSE, MPI_COMM_WORLD);

        // Se sienta
        MPI_Recv(NULL, 0, MPI_INT, CAMARERO, SENTARSE, MPI_COMM_WORLD, NULL);

        // Solicita tenedor izquierdo
        cout << "Filósofo " << id << " solicita tenedor " << izq << endl << flush;

        MPI_Ssend(NULL, 0, MPI_INT, izq, COGER, MPI_COMM_WORLD);

        // Solicita tenedor derecho
        cout << "Filósofo " << id << " solicita tenedor " << der << endl << flush;

        MPI_Ssend(NULL, 0, MPI_INT, der, COGER, MPI_COMM_WORLD);
        
        cout << "Filósofo " << id << " COMIENDO" << endl << flush;

        sleep((rand() % 3)+1);  // Comiendo

        // Suelta el tenedor izquierdo
        cout << "Filósofo " << id << " suelta tenedor " << izq << endl << flush;
        
        MPI_Ssend(NULL, 0, MPI_INT, izq, SOLTAR, MPI_COMM_WORLD);

        // Suelta el tenedor derecho
        cout << "Filósofo " << id << " suelta tenedor " << der << endl << flush;

        MPI_Ssend(NULL, 0, MPI_INT, der, SOLTAR, MPI_COMM_WORLD);

        // Se levanta
        MPI_Ssend(NULL, 0, MPI_INT, CAMARERO, LEVANTARSE, MPI_COMM_WORLD );

        cout << "Filósofo " << id << " PENSANDO" << endl << flush;

        sleep((rand()%3)+1);    // Pensando
    }
}

void Tenedor(int id){
    int filosofo;
    MPI_Status status;

    while(true){
        // Espera un peticion desde cualquier filosofo vecino ...
        MPI_Recv(NULL, 0, MPI_INT, MPI_ANY_SOURCE, COGER, MPI_COMM_WORLD, &status);

        // Recibe la peticion del filosofo ...
        filosofo = status.MPI_SOURCE;

        cout << "Tenedor " << id << " recibe petición de filósofo " << filosofo << endl << flush;

        // Espera a que el filosofo suelte el tenedor...
        MPI_Recv(NULL, 0, MPI_INT, filosofo, SOLTAR, MPI_COMM_WORLD, &status);

        cout << "Tenedor " << id << " recibe liberación de filósofo " << filosofo << endl << flush; 
    }
}

void Camarero(){
    int filosofo, sentados=0;
    MPI_Status status;

    while (true){
        if (sentados < NUM_FILO-1)                                              // Para evitar interbloqueos, no pueden estar todos sentados
            MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);    // Puede sentarse o levantarse
        else
            MPI_Probe(MPI_ANY_SOURCE, LEVANTARSE, MPI_COMM_WORLD, &status);     // Solo puede levantarse

        filosofo = status.MPI_SOURCE;

        if (status.MPI_TAG == SENTARSE){ // se le deja sentarse
            MPI_Recv(NULL, 0, MPI_INT, filosofo, SENTARSE, MPI_COMM_WORLD, &status);
            sentados++;
            MPI_Send(NULL, 0, MPI_INT, filosofo, SENTARSE, MPI_COMM_WORLD);

            cout << "Filósofo " << filosofo << " se sienta, hay " << sentados << " filósofos sentados\n";
        }
        else if (status.MPI_TAG == LEVANTARSE){ // Se levanta
            MPI_Recv(NULL, 0, MPI_INT, filosofo, LEVANTARSE, MPI_COMM_WORLD, &status);
            sentados--;

            cout << "Filósofo " << filosofo << " se levanta, hay " << sentados << " filósofos sentados\n";
        }
    }
}

int main(int argc,char** argv ){
    int rank,size;

    srand(time(0));

    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );

    if(size != 2*NUM_FILO+1){
        if(rank == 0) 
            cout << "El número de procesos debe ser " << 2*NUM_FILO+1 << endl << flush;
    }
    else{
        if (rank == CAMARERO)
            Camarero();
        else if ((rank%2) == 0)     // Los pares son Filosofos
            Filosofo(rank,size-1);  // No contamos al camarero
        else                        // Los impares son Tenedores 
            Tenedor(rank);
    }

    MPI_Finalize();
    return 0;
}  

