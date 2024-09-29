#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fenv.h>

#define FLOOR 0
#define CEIL 1

union doubleHex{
    uint64_t binario;
    double valor;
};

struct retorno {
    int n;
    double somaAnterior;
    double somaAtual;
    int flops;
};

double num(int k, struct retorno *retorno){
    unsigned long long potencia2 = 1;
    for (int i = 1; i <= k; i++){
        potencia2 *= 2;
    }
    double potenciaK = 1;
    if (k > 1){
        double result = 1;
        for(int i=1; i<=k; ++i){
            result *= i;   
            retorno->flops++;
        }
        potenciaK = result * result;
        retorno->flops++;
    }
    double numerador = potencia2 * potenciaK;
    return numerador;
}

double den (int k, struct retorno *retorno){
    int dentro = 2 * k +1;
    double denominador = 1;
    if (dentro > 1){
        for(int i=1; i<=dentro; ++i){
            denominador *= i;       
            retorno->flops++;       
        }
    } 
    return denominador;
}

double somatorio (int k, struct retorno *retorno){
    double soma = 1.0;
    retorno->n = 0;
    for (int i =1; i<=k;i++){
        double numerador = num(i, retorno);
        double denominador = den(i, retorno);
        double valor = numerador / denominador;
        soma = soma + valor;
        retorno->flops++;
        retorno->n++;
    }
    return soma;
}

double calculoPi (double tolerancia, struct retorno *retorno){
    double it1;
    it1= 1.0;
    double it2;
    int k = 1;
    it2 = somatorio (k,retorno);
    while (fabs(it2 - it1) > tolerancia){
        retorno->flops++;
        k = k + 1;
        it1 = it2;
        it2 = somatorio (k,retorno);
    }
    retorno->somaAnterior = it1;
    retorno->somaAtual = it2;
    return it2;
}

int main(){
    struct retorno retorno;
    retorno.n = 0;
    double tolerancia;
    printf ("Digite a tolerância: \t");
    scanf("%lf",&tolerancia);

    //arredondar para baixo
    fesetround(FE_DOWNWARD);
    union doubleHex piBaixo;
    piBaixo.valor = calculoPi(tolerancia,&retorno) * 2;

    //arredondar para cima
    retorno = (struct retorno){0, 0, 0.0, 0.0};
    fesetround(FE_UPWARD);
    union doubleHex piCima;
    piCima.valor = calculoPi(tolerancia,&retorno) * 2;
    retorno.flops++;

    //calculos dos resultados
    double erro_aproximado = fabs((retorno.somaAtual - retorno.somaAnterior)/retorno.somaAtual);
    double erro_exato = fabs (piCima.valor - M_PI);
    int ulpsDiff = abs(piBaixo.binario - piCima.binario); //subtraio a representação de bits dos dois números

    //impressões
    printf ("n: %d\n",retorno.n);
    printf ("erro aproximado: %.15e\n",erro_aproximado);
    printf ("erro exato: %.15e\n",erro_exato);
    printf ("piBaixo: %.15e\n",piBaixo.valor);
    printf ("piCima: %1.15e\n",piCima.valor);
    printf ("ULPs diff: %d\n", ulpsDiff);
    printf ("Flops: %d\n",retorno.flops);
}