/******************************************************************************
Um decompressor, programa que recebe números gerados pelo primeiro programa e
visualiza-os, produzindo uma imagem em PNG.
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void dct_inverso(int N, double D[N][N], int M[N][N]);
void calcula_base(int N, double c[N][N]);
void dequantiza(int Dtil[8][8], double D[8][8]);
void le_codificacao(int Dtil[8][8]);

int main(int argc, char *argv[]){
    int width, height;
    scanf("%d %d", &width, &height);
    unsigned char (*img)[width] = malloc(width*height*sizeof(unsigned char));
    if (img == NULL) return 1;

    for (int lin = 0; lin < height; lin += 8){
        for (int col = 0; col < width; col += 8){
            int Dtil[8][8], bloco[8][8];
            double D[8][8];
            le_codificacao(Dtil);
            dequantiza(Dtil, D);
            dct_inverso(8, D, bloco);

            for (int i = 0; i < 8; i++){
                for (int j = 0; j < 8; j++){
                    img[lin+i][col+j] = (unsigned char)bloco[i][j];
                }
            }
        }
    }
    stbi_write_png(argv[1], width, height, 1, img, width);
    free(img);
    return 0;
}

void dct_inverso(int N, double D[N][N], int M[N][N]){
    double c[N][N];    
    calcula_base(N, c);
    for (int x = 0; x < N; x++) {
        for (int y = 0; y < N; y++) {
            double soma = 0;
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < N; j++) {
                    soma += D[i][j]*c[i][x]*c[j][y];
                }
            }
            int valor = round(soma) + 128;
            if (valor < 0) valor = 0;
            else if (valor > 255) valor = 255;
            M[x][y] = valor;
        }
    }
}

void calcula_base(int N, double c[N][N]){
    for (int freq = 0; freq < N; freq++){
        double modulo;
        if (freq == 0) 
            modulo = sqrt(N);
        else 
            modulo = sqrt(N/2.0);
        for (int i = 0; i < N; i++){
            c[freq][i] = cos(freq*M_PI*(i+(0.5))/N)/modulo;
        }    
    }
}

void dequantiza(int Dtil[8][8], double D[8][8]){
    int Q[8][8] = {{16, 10, 10, 16, 25, 41, 50, 61},
    {12, 13, 14, 19, 26, 58, 60, 54},
    {14, 12, 16, 24, 41, 56, 68, 57},
    {14, 16, 22, 29, 51, 87, 81, 63},
    {18, 23, 37, 56, 69, 108, 102, 76},
    {24, 34, 55, 64, 80, 105, 113, 92},
    {49, 65, 78, 87, 103, 120, 120, 100},
    {72, 93, 95, 98, 113, 100, 103, 98}};

    for (int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            D[i][j] = Dtil[i][j]*Q[i][j];
        }
    }
}

void le_codificacao(int Dtil[8][8]){
    int x, contador = 0;
    int sequencia[64];
    bool zeros = false;
    while (contador < 64 && scanf("%d", &x) == 1){
        if (!zeros){
            sequencia[contador++] = x;
            zeros = true;
        }
        else{
            for (int i = 0; i < x && contador < 64; i++){
                sequencia[contador++] = 0;
            }
        zeros = false;
        }
    }
    int total_diagonais = 8+8-1, indice = 0;
    for (int soma = 0; soma < total_diagonais; soma++){
        for (int i = 0; i < 8; i++){
            int j = soma - i;
            if (j < 0 || j >= 8) continue;
            Dtil[i][j] = sequencia[indice++];
        }
    }
}
