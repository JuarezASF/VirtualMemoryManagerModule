
/********************************************************************
Universidade de Brasília - UnB
Departamento de Ciência da Computação - CIC
Alunos: Gabriel Franklin Braz de Medeiros - 15/0126387
        Juarez Aires Samaio Filho 		  - 11/0032829

Trabalho Prático para a disciplina de Sistemas Operacionais (01/2016)

Instruções:
- Para compilar digite g++ processo_usuario.cpp -o programa
- Para executar digite ./programa pag_processo_0.txt
********************************************************************/

#include<iostream>
#include<vector>
#include<string>
#include<cstdio>
#include <fstream>
#include<cstdlib>
#include <unistd.h>


#define ERROR1 "Passagem incorreta de parâmetros\n"

using namespace std;

/*
@Function - main
Função principal do processo_usuario
Realiza a leitura do arquivo e a chamada da rotina referencia
*/

int main(int argc, char** argv){
	ifstream inputFile;
	string linha;
	int i;
	int j;
	int k;


	if(argc != 2){
		printf(ERROR1);
		return 1;
	}else{
		inputFile.open(argv[1]); //Leitura do arquivo passado via linha de comando
		if(!inputFile.is_open()){
			printf(ERROR1);
			return 1;
		}
		
	}
	
	string aux;
	aux.clear();
	i=0;
	j=0;
	k=0;
	while(getline(inputFile,linha)){
		j = 0;
		
		for(k = 0;k<linha.size();k++){
			while(linha[j] != ',' && linha[j] != '\0' && j < linha.size()){
				aux += linha[j];
				j++;
			}
			j++;
		
			
			
			aux.clear();
		}
		
		i++;
		linha.clear();
	}

	
	return 0;
}
