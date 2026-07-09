#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <complex.h>

#define ITER_MAX 10000
#define N 256
#define TOLERANCIA 1e-13

double complex eval_poly(double a[], int grau, double complex x);
bool eval_error(double a[], int grau, double complex x);
double complex root(double a[], double b[], double complex z0, int grau, bool *converge);
bool search (double complex roots[], double complex root, int size);

int main()
{
    double fx [501] = {0};
    double dfx [500] = {0}; //possivel erro de memoria
    int grau, grau_max = 0; 
    double coef;

    //ler e formar duas listas com fx e sua derivada dfx
    while (scanf("%d %lf", &grau, &coef) == 2)//possivel erro de formatacao?
    {
        fx[grau] = coef; 
        if (grau > 0)
        {
            dfx[grau-1] = grau*coef;
        }
        if (grau > grau_max)
        {
            grau_max = grau; 
        }
    }
    double complex roots [grau_max];
    int size_roots = 0; 
    
    //iterar sobre os valores do conjunto
    for (int r = 0; r <= N; r++)
    {
        for (int s = 0; s <= N; s++)
        {
            bool converge = false;
            double real = -2.0 + 4.0*r/N;
            double imag = -2.0 + 4.0*s/N;
            double complex z0 = real + imag*I;
            double complex raiz =  root(fx, dfx, z0, grau_max, &converge);
            
            if (converge && !search(roots, raiz, size_roots)){
                printf("%.12f %.12f \n", creal(raiz), cimag(raiz));
                if (size_roots == grau_max) break;
                roots[size_roots] = raiz; 
                size_roots ++;
            }
        }
    }
}

double complex eval_poly(double a[], int grau, double complex x)//funcao que resolve polinomios, tem resultado imaginario?
{
    double complex s = 0;
    for (int i = grau; i >=0; i--){
        s = s*x + a[i];
    }
    return s;
}

bool eval_error(double a[], int grau, double complex x)
{
    double s = 0;
    for (int i = grau; i >=0; i--){
        s = s*cabs(x) + fabs(a[i]);//possivel erro de interpretacao
    }
    double complex pz = eval_poly(a, grau, x);
    return cabs(pz) <= TOLERANCIA * s;;
}

//funcao que implementa metodo de newton e devolve raiz para um z0 COMPLEXO
double complex root(double a[], double b[], double complex z0, int grau, bool *converge)
{
    double complex aprox = 0;

    for (int i = 0; i < ITER_MAX; i++){
        if (cabs(eval_poly(b, grau-1, z0)) < TOLERANCIA){
            *converge = false;
            return z0;
        }
        aprox = z0 - eval_poly(a, grau, z0)/eval_poly(b, grau-1, z0);
        if (eval_error(a, grau, aprox)){
            *converge = true;
            return aprox;
        }
        if (cabs(aprox - z0) < 1e-15){
            *converge = false;
            return aprox;
        }
        z0 = aprox;
    }
    *converge = false;
    return z0;
} 

bool search (double complex roots[], double complex root, int size)
{
    for (int i = 0; i < size; i++){
        if (fabs(creal(roots[i])-creal(root)) <= 1e-12 && fabs(cimag(roots[i])-cimag(root)) <= 1e-12)
            return true;
    }
    return false;
}
