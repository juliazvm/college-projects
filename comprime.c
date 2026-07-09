/******************************************************************************
Ao preencher esse cabeçalho com o meu nome e o meu número USP, declaro que sou
o único autor e responsável por esse programa. Todas as partes originais desse
Exercício-Programa (EP) foram desenvolvidas e implementadas por mim seguindo
as instruções desse EP e que portanto não constituem desonestidade acadêmica
ou plágio.
Declaro também que sou responsável por todas as cópias desse programa e que eu
não distribuí ou facilitei a sua distribuição. Estou ciente que os casos de
plágio e desonestidade acadêmica serão tratados segundo os critérios
divulgados na página da disciplina.
Entendo que EPs sem assinatura não serão corrigidos e, ainda assim, poderão
ser punidos por desonestidade acadêmica.
Nome : Julia de Azevedo Mendonca 
NUSP : 17869668
Turma: 01
Prof.: Yoshi
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void dct (int N, int M[N][N], double D[N][N]);
void calcula_base(int N, double c[N][N]);
void quantiza(double D[8][8], int Dtil[8][8]);
void imprime_codificacao(int Dtil[8][8]);

int main(int argc, char *argv[]){
    int width, height, channels;
    unsigned char *img = stbi_load(argv[1], &width, &height, &channels, 1);
    if (img == NULL) return 1;
    
    int (*imagem)[width] = malloc(width*height*sizeof(int));
    if (imagem == NULL){
        stbi_image_free(img);
        return 1;
    }
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            imagem[i][j] = (int)img[i*width + j] - 128;
        }
    }
    stbi_image_free(img);

    if (width % 8 != 0) width -= width % 8;
    if (height % 8 != 0) height -= height % 8;
    printf("%d %d\n", width, height);

    for (int lin = 0; lin < height; lin += 8){
        for (int col = 0; col < width; col += 8){
            int bloco[8][8];
            for (int i = 0; i < 8; i++){
                for (int j = 0; j < 8; j++){
                    bloco[i][j] = imagem[lin+i][col+j];
                }
            }
            double D[8][8];
            int Dtil[8][8];
            dct(8, bloco, D);
            quantiza(D, Dtil);
            imprime_codificacao(Dtil);
        }
    }
    stbi_image_free(imagem);
    return 0;
}

void dct (int N, int M[N][N], double D[N][N]){
    double c[N][N];
    calcula_base(N, c);
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            double soma = 0;
            for (int x = 0; x < N; x++){
                for (int y = 0; y < N; y++){
                    soma += M[x][y]*c[i][x]*c[j][y];
                }
            }
            D[i][j] = soma;
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

void quantiza(double D[8][8], int Dtil[8][8]){
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
            Dtil[i][j] = round(D[i][j]/Q[i][j]);
            if (Dtil[i][j] < -128) Dtil[i][j] = -128;
            else if (Dtil[i][j] > 127) Dtil[i][j] = 127;
        }
    }
}

void imprime_codificacao(int Dtil[8][8]){
    bool primeiro = true;
    int total_diagonais = 8+8-1, contador = 0;
    for (int soma = 0; soma < total_diagonais; soma++){
        for (int i = 0; i < 8; i++){
            int j = soma - i;
            if (j < 0 || j >= 8) continue;
            if (primeiro){
                printf("%d ", Dtil[i][j]);
                primeiro = false;
            }
            else{ 
                if(Dtil[i][j]==0){
                    contador++; 
                }
                else{
                    printf("%d ", contador);
                    printf("%d ", Dtil[i][j]);
                    contador = 0;
                }
            }
        }
    }
    printf("%d\n", contador);
}
