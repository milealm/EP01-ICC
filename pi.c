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

    double resultNum;
    int dentroDen;
    double resultDen;
};

double num(int k, struct retorno *retorno){
    unsigned long long potencia2 = 1;
    for (int i = 1; i <= k; i++){
        potencia2 *= 2;
    }
    double potenciaK = 1;
    if (k > 1){
        double result = retorno->resultNum * k;
        retorno->resultNum = result;
        potenciaK = result * result;
        retorno->flops = retorno->flops + 2;
    }
    double numerador = potencia2 * potenciaK;
    retorno->flops++;
    return numerador;
}

double den (int k, struct retorno *retorno){
    int dentro = 2 * k +1;
    double denominador = 1;
    if (dentro > 1){
        for(int i = retorno->dentroDen+1; i<=dentro; ++i){
            if (i == retorno->dentroDen+1 && retorno->dentroDen != 0){
                denominador *= (retorno->resultDen * i);
            }
            else if (retorno->dentroDen == 0){
                denominador *= i;
            }
            else{
                denominador *= i;  
            }
            retorno->flops++;       
        }
    } 
    retorno->dentroDen = dentro;
    retorno->resultDen = denominador;
    return denominador;
}

double somatorio (int k, struct retorno *retorno){
    retorno->n = 0;
    double numerador, denominador;
    numerador = num(k, retorno);
    denominador = den(k, retorno);
    double valor = numerador / denominador;
    double soma = retorno->somaAnterior + valor;
    retorno->flops = retorno->flops + 2;
    retorno->n++;
    return soma;
}

double calculoPi (double tolerancia, struct retorno *retorno){
    double it1;
    it1= 1.0;
    double it2;
    int k = 1;
    retorno->resultNum = 1;
    retorno->dentroDen = 0;
    retorno->resultDen = 1;
    it2 = somatorio (k,retorno);
    while (fabs(it2 - it1) > tolerancia){
        retorno->flops++; //pela comparação
        k = k + 1;
        it1 = it2;
        retorno->somaAnterior = it1;
        it2 = somatorio (k,retorno);
    }
    retorno->n=k;
    retorno->somaAtual = it2;
    it2 = it2 + 1.0;
    retorno->flops++;
    return it2;
}

int main(){
    struct retorno retorno;
    retorno.n = 0;
    double tolerancia;
    scanf("%lf",&tolerancia);

    //arredondar para baixo
    retorno = (struct retorno){0.0};
    fesetround(FE_DOWNWARD);
    union doubleHex piBaixo;
    piBaixo.valor = calculoPi(tolerancia,&retorno) * 2;

    //arredondar para cima
    retorno = (struct retorno){0.0};
    fesetround(FE_UPWARD);
    union doubleHex piCima;
    piCima.valor = calculoPi(tolerancia,&retorno) * 2;
    retorno.flops++;

    //calculos dos resultados
    union doubleHex erro_aproximado;
    erro_aproximado.valor = fabs((retorno.somaAtual - retorno.somaAnterior)/retorno.somaAtual);
    union doubleHex erro_exato;
    erro_exato.valor = fabs (piCima.valor - M_PI);
    int ulpsDiff = abs(piBaixo.binario - piCima.binario); //subtraio a representação de bits dos dois números

    //impressões
    printf ("%d\n",retorno.n);
    printf ("%.15e %08lX\n",erro_aproximado.valor, erro_aproximado.binario);
    printf ("%.15e %08lX\n",erro_exato.valor, erro_exato.binario);
    printf ("%.15e %08lX\n",piBaixo.valor,piBaixo.binario);
    printf ("%1.15e %08lX\n",piCima.valor,piCima.binario);
    printf ("%d\n", ulpsDiff);
    printf ("%d\n",retorno.flops);
}