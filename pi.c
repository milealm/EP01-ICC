#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define FLOOR 0
#define CEIL 1

union doubleHex{
    uint64_t binario;
    double valor;
};

int num(int k){
    int potencia2 = 1;
    //calculando 2^k no numerador
    for (int i = 1; i <= k; i++){
        potencia2 *= 2;
        //printf ("potencia2: %d\n",potencia2);
    }
    int potenciaK = 1;
    if (k > 1){
    //calculando k
        printf ("k é maior que 1\n");
        int result = 1;
        for(int i=1; i<=k; ++i){
            result *= i;   
        }
        //calculando o resultado de k! ao quadrado
        potenciaK = result * result;
        printf ("\tpotenciaK: %d\n",potenciaK);
    }
    int numerador = potencia2 * potenciaK;
    printf ("\tNumerador: %d\n",numerador);
    return numerador;
}

int den (int k){
    int dentro = 2 * k +1;
    int denominador = 1;
    if (dentro > 1){
        for(int i=1; i<=dentro; ++i){
            denominador *= i;              
        }
    }
    printf ("\tDenominador: %d\n",denominador);
    return denominador;
}

double somatorio (int k, int arredondar){
    double soma = 0.0;
    int count = 0;
    printf ("Iteração de um somatorio até k=%d\n",k);
    for (int i =0; i<=k;i++){
        printf ("\tcount :%d",count);
        int numerador = num(k);
        int denominador = den(k);
        double valor = 0.0;
        valor = (double) numerador / denominador;
        if (arredondar == FLOOR){
            valor = floor(valor * 1e15); //arredondar para baixo em relação às casas decimais
            valor= valor / 1e15;
        }
        else{
            valor = ceil(valor * 1e15) / 1e15; //arredondar para cima em relação às casas decimais
        }
        printf ("\tValor (k eh %d): %1.15f \n",k,valor);
        soma = soma + valor;
        printf ("\tNova Soma: %1.15f e %.15e\n",soma,soma);
        count ++;
    }
    return soma;
}

double calculoPi (double tolerancia, int arredondar){
    double it1 = 0.0; //guardar valor da iteração atual
    double it2; //guardar valor da iteração anterior
    int k = 0; //valor pelo qual vou ficar iterando
    //para k = 0
    it2 = somatorio (k,arredondar);
    printf ("fiz a primeira iteração\n");
    while (it2 - it1 > tolerancia){
        printf ("ainda é menor: %1.15f\n\n",it2-it1);
        k = k + 1;
        it1 = it2;
        it2 = somatorio (k,arredondar);
    }
    return it2;

}

int main(){
    double tolerancia;
    printf ("Digite a tolerância: \t");
    scanf("%lf",&tolerancia);
    int arredondar = FLOOR;
    double pi = calculoPi(tolerancia,arredondar);
    printf ("pi: %1.15f e %.15e",pi,pi);
}