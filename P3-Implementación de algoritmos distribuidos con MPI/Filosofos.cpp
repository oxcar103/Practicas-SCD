#include "mpi.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>

#define SOLTAR 0
#define COGER 1
#define NUM_FILO 5

using namespace std;

void Filosofo(int id, int nprocesos){
    int izq = (id+1)%nprocesos;
    int der = (id+(nprocesos-1))%nprocesos;   

    while(true){
        if(id == 0){    // Un proceso, por ejemplo el primero, los pide al revés para evitar interbloqueos
            //Solicita tenedor derecho
            cout << "Filósofo " << id << " solicita tenedor " << der << endl << flush;

            MPI_Ssend(NULL, 0, MPI_INT, der, COGER, MPI_COMM_WORLD);

            //Solicita tenedor izquierdo
            cout << "Filósofo " << id << " solicita tenedor " << izq << endl << flush;

            MPI_Ssend(NULL, 0, MPI_INT, izq, COGER, MPI_COMM_WORLD);
        }
        else{
            //Solicita tenedor izquierdo
            cout << "Filósofo " << id << " solicita tenedor " << izq << endl << flush;

            MPI_Ssend(NULL, 0, MPI_INT, izq, COGER, MPI_COMM_WORLD);

            //Solicita tenedor derecho
            cout << "Filósofo " << id << " solicita tenedor " << der << endl << flush;

            MPI_Ssend(NULL, 0, MPI_INT, der, COGER, MPI_COMM_WORLD);
        }

        cout << "Filósofo " << id << " COMIENDO" << endl << flush;

        sleep((rand() % 3)+1);  // Comiendo

        //suelta el tenedor izquierdo
        cout << "Filósofo " << id << " suelta tenedor " << izq << endl << flush;
        
        MPI_Ssend(NULL, 0, MPI_INT, izq, SOLTAR, MPI_COMM_WORLD);

        //suelta el tenedor derecho
        cout << "Filósofo " << id << " suelta tenedor " << der << endl << flush;

        MPI_Ssend(NULL, 0, MPI_INT, der, SOLTAR, MPI_COMM_WORLD);

        cout << "Filósofo " << id << " PENSANDO" << endl << flush;

        sleep((rand()%3)+1);    // Pensando
    }
}

void Tenedor(int id, int nprocesos){
    int buf, filosofo;
    MPI_Status status;

    while(true){
        // Espera un peticion desde cualquier filosofo vecino ...
        MPI_Recv(NULL, 0, MPI_INT, MPI_ANY_SOURCE, COGER, MPI_COMM_WORLD, &status);

        // Recibe la peticion del filosofo ...
        filosofo = status.MPI_SOURCE;

        cout << "Tenedor " << id << " recibe petición de filósofo " << filosofo << endl << flush;

        // Espera a que el filosofo suelte el tenedor...
        MPI_Recv(NULL, 0, MPI_INT, filosofo, SOLTAR, MPI_COMM_WORLD, &status);

        cout << "Tenedor " << id << " recibe liberacion de filósofo " << filosofo << endl << flush; 
    }
}

int main(int argc,char** argv ){
    int rank,size;

    srand(time(0));

    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );

    if(size != 2*NUM_FILO){
        if(rank == 0) 
            cout << "El número de procesos debe ser " << 2*NUM_FILO << endl << flush;
    }
    else{
        if ((rank%2) == 0)  
            Filosofo(rank,size);    // Los pares son Filosofos 
        else
            Tenedor(rank,size);     // Los impares son Tenedores 
    }

    MPI_Finalize();
    return 0;
}  

