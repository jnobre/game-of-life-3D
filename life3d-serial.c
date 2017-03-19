#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>



#define DEAD 46
#define ALIVE 45
#define TRUE 1
#define FALSE 0
//#define DEBUG //uncomment this line to add debug messages
#define BUF_SIZE 4048
#define GENS 1000
#define SIZE 500
#define	FILE_MODE	( S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH )

typedef struct _living_cells living_cells;

struct _living_cells{
	int x;
	int y;
	int z;
	int generation;
    char life;
};

int num_gen,
    size_cube,
    generations;
living_cells *livingCell;
char ***matrix;
char ***matrix_b;

void prinfLivescell( ) {
    int i = 0;
    for( i = 0 ; i < size_cube ; i++ ) {
        printf( "%d %d %d\n" , livingCell[ i ].x , livingCell[ i ].y , livingCell[ i ].z );
    }
}

int erro( char* text ) {
    printf( "Erro:%s\n" , text );
    exit( -1 );
}

int rand_index( ) {
    return rand( ) % size_cube;
}

int is_alive( int *** matrix , int x , int y , int z ) {
    if(  matrix[ x ][ y ][ z ] == ALIVE ) 
        return TRUE;
    return FALSE;
}

int is_in_range( int x , int y , int z ) {
    if( x < 0 || y < 0 || z < 0 || x >= size_cube || y >= size_cube || z >= size_cube )
        return FALSE;
    return TRUE;
}

void initMatriz( ) { 
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

void printMatrix( ) {
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

int convert_cord( int indice ) {
    if( indice == 0 )
        return size_cube - 1;
    else
        return ( indice - 1 ) % size_cube;
}

void swapMatrix( int generation ) {
    int i, j, k;
    char tmp;
    for( i = 0 ; i < size_cube ; i++ ) {
        for( j = 0 ; j < size_cube ; j++ ) {
            for( k = 0 ; k < size_cube ; k++ ) {
                matrix[ i ][ j ][ k ] = matrix[ i ][ j ][ k ] ^ matrix_b[ i ][ j ][ k ];
                matrix_b[ i ][ j ][ k ] = matrix[ i ][ j ][ k ] ^ matrix_b[ i ][ j ][ k ];
                matrix[ i ][ j ][ k ] = matrix[ i ][ j ][ k ] ^ matrix_b[ i ][ j ][ k ];
                
            }
        }
    }
}

void evolve( int generation ) {
    int i, j, k, sum;

    for( i = 0 ; i < size_cube ; i++ ) {
        for( j = 0 ; j < size_cube ; j++ ) {
            for( k = 0 ; k < size_cube ; k++ ) {
                int x , y, z;
                x = convert_cord( i );
                y = convert_cord( j );
                z = convert_cord( k );
                //printf("(%d,%d) (%d,%d) (%d,%d)\n",i,x,j,y,k,z);
                printf("(%d,%d) vizinhos => (%d,%d) (%d,%d) (%d,%d) (%d,%d) (%d,%d) (%d,%d) (%d,%d) (%d,%d)\n",i,j,x,y,x,j,x,((j+1)%size_cube),i,y,i,(j+1)%size_cube,(i+1)%size_cube,y,(i+1)%size_cube,j,(i+1)%size_cube,(j+1)%size_cube);
                sum =
                    matrix[ x ][ j ][ z ] +
                    matrix[ i ][ y ][ z ] +
                    matrix[ i ][ ( j + 1 ) % size_cube ][ z ] +
                    matrix[ ( i + 1 ) % size ][ j ][ z ] +
                    matriz[ i ][ j ][ ( k + 1 ) % size_cube ] +
                    matriz[ i ][ j ][ ( k - 1 ) % size_cube ];

                if( sum < 2 || sum > 3 )
                    matrix_b[ i ][ j ][ z ] = DEAD;
                else if( sum == 3 )
                    matrix_b[ i ][ j ][ z ] = ALIVE;
                else
                    matrix_b[ i ][ j ][ z ] = matrix[ i ][ j ][ z ];
                
            }
        }
    }


    swapMatrix( generation );
 
}

void work( ) {
    int i;

    for( i = 0 ; i <  generations ; i++ ) {
        evolve( i );  
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

    generations = argv[ 2 ];
    printf(" generations =>  %d \n" , generations );
    f = fopen( argv[ 1 ] , "r" );
    if ( !f ) {
        perror( "File open error!\n" );
        return 1;
    }

    fscanf(f, "%d" , &size_cube); 

    initMatriz( ); //build matrix

    livingCell = ( living_cells * ) malloc( sizeof( living_cells ) * size_cube );  
    memset( livingCell , 0 , size_cube * sizeof( living_cells ) ); //initialize matrix
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

    work( );

    printf( "\nGame of Life\n" );

    return 0;
    
}

