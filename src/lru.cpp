#include<stdio.h>
#include <stdlib.h>

#define SIZE 10

int full = 0; //Verifica se todos os frames j� foram preenchidos por p�ginas
int *a, n;
int frame[SIZE];
int ctr[SIZE] = {0}; //Vetor contendo um contador para fazer a associa��o com cada p�gina
static int f = 0; //Contador para servir como tempo de refer�ncia
int repptr;
int count = 0;

int Display();

int LRUalgorithm();

int PageReplace(int element);

int Pagefault(int element);

int Search(int element);

int main() {

    int i;
    FILE *fp;
    int caractere;
    fp = fopen("Input.txt", "r");
    printf("Digite o numero de paginas a serem referenciadas: ");
    scanf("%d", &n);
    a = (int *) malloc(n * sizeof(int));
    rewind(fp);
    for (i = 0; i < n; i++)
        fscanf(fp, "%d", &a[i]);
    printf("\nElementos\n");
    for (i = 0; i < n; i++)
        printf("%d  ", a[i]);
    printf("\n\n");

    for (i = 0; i < n; i++) {
        f++;//Tempo de refer�ncia
        PageReplace(a[i]);
        if (Search(a[i]) != 1) {
            Pagefault(a[i]);
            Display();
            count++;
        }

    }

    printf("\nNumero de page faults do processo: %d\n", count);

    free(a);
    return 0;

}


/*
Fun��o Display
Fun��o respons�vel por imprimir na tela a configura��o atual do frame
*/
int Display() {
    int i;
    printf("\nElementos contidos no frame\n");
    for (i = 0; i < full; i++)
        printf("%d \n", frame[i], f);

}

/*
Fun��o LRU
Fun��o respons�vel por implementar o algoritmo LRU (Least Recently Used)
 */
int LRUalgorithm() {
    int i, min = 0;

    for (i = 0; i < SIZE; i++)
        if (ctr[min] > ctr[i])
            min = i;
    repptr = min;
    return repptr;
}

/*
Fun��o PageReplace
Fun��o respons�vel por realizar a substitui��o de p�ginas
 */

int PageReplace(int element) {

    int temp;
    repptr = LRUalgorithm();
    temp = frame[repptr];
    frame[repptr] = element;
    ctr[repptr] = f;
    return temp;

}

/*
Fun��o Pagefault
Fun��o respons�vel por verificar a ocorr�ncia de um page fault
 */

int Pagefault(int element) {
    if (full != SIZE) {
        ctr[full] = f;//Seta o contador para o tempo de refer�ncia
        frame[full++] = element;
    }
    else
        printf("The page replaced is %d", PageReplace(element));
}

/*
Fun��o search
Realiza a busca de um elemento em um frame
 */
int Search(int element) {
    int i, flag = 0; // Esse flag serve para verificar se o elemento foi referenciado recentemente ou n�o

    if (full != 0) {

        for (i = 0; i < full; i++)

            if (element == frame[i]) {
                flag = 1;
                ctr[i] = f;  //Se n�o ocorreu page fault, mas a p�gina foi referenciada, o contador deve ser setado para o tempo de refer�ncia

                break;

            }
    }

    return flag;

}

