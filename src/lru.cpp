#include<stdio.h>
#include <stdlib.h>

#define SIZE 10

/* Variavéis globais */
int full=0; //Verifica se todos os frames já foram preenchidos por páginas
int *a, n; 
int frame[SIZE];
int ctr[SIZE]={0}; //Vetor contendo um contador para fazer a associação com cada página
static int f=0; //Contador para servir como tempo de referência
int repptr;
int count=0;

/*Protótipos de funções */
int Display();
int LRUalgorithm();
int PageReplace(int element);
int Pagefault(int element);
int Search(int element);

/*Função main:
Função responsável por chamar a rotina de substituição de páginas, além da função para verificar se ocorreu page fault
*/
int main()

{

   int i;
    FILE *fp;
    int caractere;
    fp=fopen("Input.txt","r");
    printf("Digite o numero de paginas a serem referenciadas: ");
    scanf("%d",&n);
    a = (int *)malloc(n * sizeof(int));
    rewind(fp);
    for(i=0;i<n;i++)
    fscanf(fp,"%d",&a[i]);
    printf("\nElementos\n");
    for(i=0;i<n;i++)
    printf("%d  ",a[i]);
    printf("\n\n");
    
	for(i=0;i<n;i++)

    {
	f++;//Tempo de referência
        PageReplace(a[i]);
                    if(Search(a[i])!=1)
                    {Pagefault(a[i]);
                  Display();
                    count++;
					}
					
    }

    printf("\nNumero de page faults do processo: %d\n",count);
         
free(a);
return 0;

}


/*
Função Display
Função responsável por imprimir na tela a configuração atual do frame
*/
int Display()

{int i;
printf("\nElementos contidos no frame\n");
for(i=0;i<full;i++)
printf("%d \n",frame[i],f);

}

/*
Função LRU
Função responsável por implementar o algoritmo LRU (Least Recently Used)
 */
int LRUalgorithm()
{
int i,min=0;

for(i=0;i<SIZE;i++)
if(ctr[min]>ctr[i])
min=i;
repptr=min;
return repptr;
}

/*
Função PageReplace
Função responsável por realizar a substituição de páginas
 */

int PageReplace(int element)

{

 int temp;
 repptr=LRUalgorithm();
 temp=frame[repptr];
 frame[repptr]=element;
 ctr[repptr]=f;
  return temp;   

}

/*
Função Pagefault
Função responsável por verificar a ocorrência de um page fault
 */

int Pagefault(int element) 
 {if(full!=SIZE) 
 {ctr[full]=f;//Seta o contador para o tempo de referência
                frame[full++]=element; 
 } 
else 
 printf("The page replaced is %d",PageReplace(element)); 
 } 

/*
Função search
Realiza a busca de um elemento em um frame
 */
int Search(int element)
{
int i,flag=0; // Esse flag serve para verificar se o elemento foi referenciado recentemente ou não

if(full!=0)

 {

    for(i=0;i<full;i++)

    if(element==frame[i])

    {               
	flag=1;ctr[i]=f;  //Se não ocorreu page fault, mas a página foi referenciada, o contador deve ser setado para o tempo de referência

    break;

    }
}

 return flag;   

}

