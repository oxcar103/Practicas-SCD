#include "mpi.h"
#include <iostream>
#include "math.h"
#include <time.h>
#include <stdlib.h>

#define TAG_PROD 1          // Tag de productor
#define TAG_CONS 2          // Tag de consumidor
#define NUM_PROD 5          // Número de productores
#define NUM_CONS 4          // Número de consumidores
#define ID_BUFFER NUM_PROD  // El proceso Buffer está entre los productores y los consumidores
#define ITERS 20            // Número de elementos a producir
#define TAM 5               // Tamaño del buffer

using namespace std;
void productor(int id){
    for (unsigned int i = id; i < ITERS; i+=NUM_PROD){ 
        cout << "Productor " << id << " produce valor " << i << endl << flush;

        MPI_Ssend(&i, 1, MPI_INT, ID_BUFFER, TAG_PROD, MPI_COMM_WORLD);
    }
}

void buffer(){
    int value[TAM], peticion, pos=0,rama;
    MPI_Status status;

    for(unsigned int i=0; i < ITERS * 2; i++){  
        if (pos == 0)
            rama = 0;       // El consumidor no puede consumir
        else if (pos==TAM)
            rama = 1;       // El productor no puede producir   
        else{               // Ambas guardas son ciertas
            MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

            if (status.MPI_TAG == TAG_PROD)
                rama = 0;
            else
                rama = 1;
        }

        switch(rama){
            case 0:
                MPI_Recv( &value[pos], 1, MPI_INT, MPI_ANY_SOURCE, TAG_PROD, MPI_COMM_WORLD,&status);

                cout<< "Buffer recibe " << value[pos] << " de Productor " << status.MPI_SOURCE << endl << flush;  
                pos++;
                break;

            case 1:
                MPI_Recv( &peticion, 1, MPI_INT, MPI_ANY_SOURCE, TAG_CONS, MPI_COMM_WORLD,&status);
                MPI_Ssend( &value[pos-1], 1, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD);

                cout<< "Buffer envía " << value[pos-1] << " a Consumidor " << status.MPI_SOURCE << endl << flush;  
                pos--;
                break;
        }     
    }
}   

void consumidor(int id){
    int value, peticion = 1;
    float raiz;
    MPI_Status status;

    for (unsigned int i = id-(NUM_PROD+1); i < ITERS; i+=NUM_CONS){
        MPI_Ssend(&peticion, 1, MPI_INT, ID_BUFFER, TAG_CONS, MPI_COMM_WORLD);
        MPI_Recv(&value, 1, MPI_INT, ID_BUFFER, 0, MPI_COMM_WORLD,&status);

        raiz = sqrt(value);

        cout<< "Consumidor " << id << " recibe valor " << value << " de Buffer, su raíz es " << raiz << endl << flush;
    }
}

int main(int argc, char *argv[]) {
    int rank,size; 

    cout.flush();

    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );

    if (size != NUM_PROD+NUM_CONS+1){
        if(rank == 0)
            cout<< "El número de procesos debe ser " << NUM_PROD+NUM_CONS+1 << endl;

        return 0;
    }

    if (rank < ID_BUFFER)
        productor(rank);
    else if (rank == ID_BUFFER)
        buffer();
    else
        consumidor(rank);

    MPI_Finalize( );

    return 0;
}

