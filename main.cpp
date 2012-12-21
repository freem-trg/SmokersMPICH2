#include <iostream>
#include <string>
#include <stdlib.h>
#include "mpi.h"
#include <unistd.h>

using namespace std;

int table[3] = {1, 1, 1}; // [tobacoo, paper, match]
string ingrids[3] = {"tobacoo", "paper", "match"};

void show_message( string msg) {
    cout << endl << msg << endl;
}

void broker(){
        show_message("Broker watch for ingridients...");
        int num = 0;
        for ( int i = 0; i < 3; i++ ) table[i] = 0;
        cout << "And puts : ";
        while ( num < 2 ){
            int pos = random()%3;
            if ( table[ pos ] == 0 ) {
                num++;
                table[ pos ] = 1;
                cout << ingrids[ pos ] << " ";
            }
        }
}

void smoker(int num){
    switch (num){
    case 0:
        show_message("Smoker with tobacoo smokes... ");
        break;
    case 1:
        show_message("Smoker with paper smokes... ");
        break;
    case 2:
        show_message("Smoker with match smokes... ");
        break;
    }
    cout << endl;
    table[num] = 1;
    sleep(1);
}



int main( int argc, char *argv[] )
{
    //For MPI
    int pid;
    int ierr;

    //Enter the MPI:
    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &pid);

    for ( int i = 0; i < 10; i++ ){
        if ( pid == 0){
            //Broker code here:
            broker();
        }
        MPI_Bcast( &table, 3, MPI_INT, 0, MPI_COMM_WORLD);
        if ( pid != 0) {
            //Smoker code here:
            if( table[pid-1] == 0 ) {
                smoker( pid-1 );
            }
        }
        ierr = MPI_Barrier( MPI_COMM_WORLD );
    }
    MPI_Finalize();
    return 0;
}

