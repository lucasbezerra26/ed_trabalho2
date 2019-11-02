#include <stdio.h>
#include <stdlib.h>

int **function(){
    int **matriz;
    matriz = (int**)malloc(sizeof(int*)*81);
    for (int i = 0; i < 81; i++){
        matriz[i] = (int*) calloc(4,sizeof(int));
    }
    int cont=0;
    for( int i=0; i<3; i++){
        for( int z=0; z<3; z++){
            for( int x=0; x<3; x++){
                for( int y=0; y<3; y++){
                    // printf("eii");
                    matriz[cont][0] += i+1;
                    matriz[cont][1] += z+1;
                    matriz[cont][2] += x+1;
                    matriz[cont][3] += y+1;
                    cont++;
                }

            }

        }
    }
    return matriz;
}

int main(){
    int **matriz;
    // matriz = (int**)malloc(sizeof(int*)*81);
    // for (int i = 0; i < 81; i++){
    //     matriz = (int**)malloc(sizeof(int*)*4);
    // }
    
    matriz = function();
    for(int i=0; i < 81;i++){

        for(int j=0; j < 4; j++){
            printf("%d",matriz[i][j]);
        }
        printf("\n");
    }
        
    return 0;
}