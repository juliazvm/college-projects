#include <stdio.h>

int melhor(int compA, int compB, int tempoA, int tempoB, int tentA, int tentB);
void resultado(int comp, int tent, int tempo);

int main ()
{
    int comp, tent, tempo;
    int comp1, tent1, tempo1;

    scanf("%d %d %d", &comp1, &tent1, &tempo1);

    int comp2 = comp1, tent2 = tent1, tempo2 = tempo1;
    int tem_segundo = 0;
    
    while (scanf("%d %d %d", &comp, &tent, &tempo) == 3)
    {
        if (melhor(comp, comp1, tempo, tempo1, tent, tent1)) //novo e melhor que primeiro
        {
            if (comp != comp1)
            {
                comp2 = comp1, tent2 = tent1, tempo2 = tempo1;
            }
            comp1 = comp, tent1 = tent, tempo1 = tempo;
        }
        else if(!tem_segundo && comp != comp1)
        {
            comp2 = comp, tent2 = tent, tempo2 = tempo;
            tem_segundo = 1;
        }
        else if(melhor(comp, comp2, tempo, tempo2, tent, tent2) && comp != comp1)//novo nao e melhor que primeiro, mas e melhor que o segundo
        {
            comp2 = comp, tent2 = tent, tempo2 = tempo;
        } 
    }

    printf("Pole position: ");
    resultado(comp1, tent1, tempo1);
    printf("Segundo lugar: ");
    resultado(comp2, tent2, tempo2);
    
    return 0;
}

int melhor(int compA, int compB, int tempoA, int tempoB, int tentA, int tentB)// se o empate permanecer, desempata com o menor comp
{
    //tempo de A maior que de B 
    if (tempoA < tempoB)
    {
        return 1;
    }
    else if (tempoA > tempoB)
    {
        return 0; 
    }
    else if (tentA < tentB)
    {
        return 1; //em caso de empate, retorna 1 se a tentativa de A for menor que B, sendo A o "melhor"
    }
    else if(tentA > tentB)
    {
        return 0;
    }
    else 
    {
        return compA < compB;
    }
}

void resultado(int comp, int tent, int tempo)
{
    int min, seg, milisseg;

    milisseg = tempo % 1000;
    tempo /= 1000;
    seg = tempo % 60;
    min = tempo / 60;

    printf("Piloto %d, com tempo de %d %s, %d %s e %d %s (tentativa %d).\n", comp, 
        min, min == 1? "minuto": "minutos",
        seg, seg == 1? "segundo": "segundos", 
        milisseg, milisseg == 1? "milissegundo":"milissegundos",
        tent);
}