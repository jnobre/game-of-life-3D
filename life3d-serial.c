#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/fcntl.h>
#include <sys/shm.h>
#include <limits.h>
#include <sys/mman.h>


#define DEAD 46
#define ALIVE 45

#define DEBUG //uncomment this line to add debug messages
#define BUF_SIZE 4048
#define GENS 1000
#define SIZE 500
#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

typedef struct _living_cells living_cells;

struct _living_cells{
	int x;
	int y;
	int z;
	int generation;
    char life;
};

int num_gen,
    size_cube;
living_cells *livingCell;
char ***matrix;
char ***matrix_b;

void prinfLivescell( ) {
    int i = 0;
    for( i = 0 ; i < size_cube ; i++ ){
        printf( "%d %d %d\n" , livingCell[ i ].x , livingCell[ i ].y , livingCell[ i ].z );
    }
}

void initMatriz( ){ 
    int i, j, x, y, z;
    matrix      = ( char *** )  malloc( sizeof( char ** )  * size_cube );
    matrix_b    = ( char *** ) malloc( sizeof( char ** ) * size_cube );
    for ( i = 0 ; i < size_cube ; i++ ) {
        matrix[ i ]     = ( char ** ) malloc( sizeof( char * ) * size_cube );
        matrix_b[ i ]   = ( char ** ) malloc( sizeof( char * ) * size_cube );
        for( j = 0 ; j < size_cube ; j++ ) {
            matrix[ i ][ j ]    = ( char * ) malloc( sizeof( char ) );
            matrix_b[ i ][ j ]  = ( char * ) malloc( sizeof( char ) );
        }
    }

    for( x = 0 ; x < size_cube ; x++ ) {
        for( y = 0 ; y < size_cube ; y++ ) {
            for( z = 0 ; z < size_cube ; z++ ) {
                matrix[ x ][ y ][ z ] = DEAD;
                matrix_b[ x ][ y ][ z ] = DEAD;
            }
        }
    }
}

void printMatrix( ){
    int x, y, z;
    for( x = 0 ; x < size_cube ; x++ ) {
        for( y = 0 ; y < size_cube ; y++ ) {
            for( z = 0 ; z < size_cube ; z++ ) {
                if( matrix[x][y][z] == ALIVE )
                    printf( " (%d,%d,%d) => 1 ", x , y , z );
                else
                    printf( " (%d,%d,%d) => 0 ", x , y , z );
            }
            printf("\n");
        }
    }   
}

int main( int argn, char * argv[ ] ) {
    int i = 0,
        j = 0;
    FILE *f;    
    if (argn != 3 ) {
        fprintf( stderr , "Wrong parameter.\n     Please use: life3d-serial <file path> <number of generations>\n" );
        exit(-1);
    }

    f = fopen( argv[ 1 ] , "r" );
    if ( !f ) {
        perror( "File open error!\n" );
        return 1;
    }

    fscanf(f, "%d" , &size_cube); 

    initMatriz( );

    livingCell = ( living_cells * ) malloc( sizeof( living_cells ) * size_cube );  
    memset( livingCell , 0 , size_cube * sizeof( living_cells ) );
    while ( !feof ( f ) ) {
        fscanf( f , "%d %d %d" , &livingCell[ i ].x , &livingCell[ i ].y , &livingCell[ i ].z );
        matrix[ livingCell[ i ].x ][ livingCell[ i ].y ][ livingCell[ i ].z ] = ALIVE;
        i++;
    }    
    fclose( f );

    //prinfLivescell(  );
    #ifdef DEBUG
        printMatrix(  );
    #endif

    printf( "\nGame of Life\n" );

    return 0;
    
}

