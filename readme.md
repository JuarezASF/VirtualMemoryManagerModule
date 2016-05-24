### Universidade de Brasília
##### Sistemas Operacionais- 1/2016
##### Professora: Alba

Alunos

Gabriel Franklin Braz de Medeiros 15/0126387    gabriel.medeiros93@gmail.com

Juarez Aires Sampaio Filho      11/0032829      juarez.asf@gmail.com


## Trabalho 1 - Gerente de Memória Virtual

1. Sistema de Desenvolvimente

    SO
        Ubuntu 14.04.4 LTS

    Kernel:
    
        Linux 3.16.0-71-generic x86_64
        
    Compilador:
    
        g++ (Ubuntu 4.9.3-8ubuntu2~14.04) 4.9.3


2. Instruções para compilação

 O projeto consiste de diversas classes e alguns executáveis que devem ser compilados e linkados corretamente. O 
 método recomendado para buildar o projeto é utilizando cmake. Fornece-se também instruções para compilar o projeto
 pela linha de comando.

2.1 Compilação com cmake
    versão do cmake utilizada: 

        cmake version 3.5.0

    Comandos:
        apartir do diretório que contém esse readme:

            mkdir build
            cd build
            cmake ..
            make


2.2 Por linha de comando

    apartir do diretório que contém esse readme:
    
        * compile todos os arquivos
        
            cd src
            for f in *.cpp; do; echo "compiling $f"; g++ --std=c++11 $f -c; done; 
            
         * link os executáveis
         
            - servidor de alocação
            
                g++ --std=c++11 startPageAllocationServer.o AbstractProcess.o ConfigParser.o j*.o PageAllocationServer.o  ResourceManager.o -o startPageAlloc 
         
            - servidor de substituição
                
                g++ --std=c++11 startPageSubstitutionServer.o AbstractProcess.o ConfigParser.o j*.o PageAllocationServer.o  ResourceManager.o PageSubstitutionServer.o -o startPageSubst
          
            - processo usuário
            
                g++ --std=c++11 startUserProcess.o AbstractProcess.o ConfigParser.o j*.o  ResourceManager.o  UserProcess.o -o startUserProcess 
                
            - shutdown
            
                g++ --std=c++11 shutdownAll.o ConfigParser.o j*.o  ResourceManager.o   -o shutdownAll
         
         * organize as coisas
         
            mkdir ../bin
            mv *.o ../bin
            mv startPageAlloc ../bin
            mv startPageSubst ../bin
            mv startUserProcess ../bin
            mv shutdownAll ../bin
            
         Ao final os executáveis e os códigos objetos produzidos devem estar no diretório bin/

3. Executando

Os programas buscam as chaves para as estruturas compartilhadas a partir de um mesmo arquivo de configuração
"config.txt.". Por tanto, um arquivo com esse nome e com configurações válidas deve ser colocada na pasta
onde os programas serão executas. Um arquivo de exemplo é fornecido no diretório data/.

Suponha que a estrutura do projeto esteja assim:

     raiz/
        src/
        data/
        build/
            src/

e os executáveis estejam dentro de raiz/build/src. Se você estiver em raiz/ faça

     cp data/config.txt build/src
     cd build/src
     


3.1 Ordem de execução dos Programas

    Deve-se executar:
    
    1. Processo servidor de alocação de páginas
        
        ./startPageAlloc
        
    Esse proceso, por saber que é o primeiro, irá criar todos os recursos compartilhados supondo
    que eles aidna não existem. Os demais processos assumem que os recursos já foram criados.
        
    2. Processo subsitituidor de página
    
        ./startPageSubst
        
    3. Processos de usuário
    
        ./startUserProcess page_process_N.txt
        
     O processo de usuário lê do primeiro argumento da linha de comando o nome do arquivo com
     as informações das páginas que deve referencias.
     
     4. Processo shutdown
     
        ./shutdownAll

4. Descrição da comunicação entre os processos.

4.1 Processo usuário <-> servidor de alocação de mensagem

    Os processos de usuário mandam pedidos de alocação para o processo
    servidor alocador de páginas por meio de uma fila de mensagem. A
    mensagem que o processo de usuário envia contém o PID do processo
    requerente. Para um dado pedido, o servidor aloca um frame e
    coloca uma mensagem de reposta em uma segunda fila de mensagens.
    Essa mensagem de resposta possui um identificador de mensagem
    igual a PID do processo a que se destina. A mensagem de resposta
    também indica se o servidor de memória passou por um pagefault
    durante aquele pedido. O servidor de alocação de páginas lê a fila
    de entrada sequencialmente.



4.2 servidor de alocação de mensagem  <-> servidor de substituição de páginas

    Ambos os servidores manipulam uma tabela contendo informação dos
    frames. Essa tabela está em memória compartilhada. Para
    sincronizar o acesso a memória existe um semáforo. O processo que
    quer acessar a tabela ( para leitura ou para escrita ) adquire o
    semáforo e faz as operações que são necessárias. Quando não
    precisa mais da tabela, o processo libera o semáforo.


4.2 Servidor de substituição

    O processo de substituição é ativado periodicamente. A chamda de
    sistema usleep é utilizada para indicar tempo de espera com
    precisão de microsegundos.

4.3 Processo de shutdown <-> outros processos

    Cada processo registra em seu construtor uma resposta ao sinal
    SIGUSR2. Ao receber esse sinal os processos de usuário, servidor
    de alocação e substituição de páginas encerram suas atividades
    imediatamente por meio da chamada exit.
    
    Para que isso ocorra, o processo de shutdown precisa conhecer o
    PID dos processos que precisa encerrar. Para isso se utiliza ourta
    estrutura compartilhada: uma tabela em memória compartilhada
    contendo informação do PID de cada processo e informação de
    pagefault. 
    
    Nessa tabela a primeira entrada é referente ao processo servidor
    alocador; a segunda, ao processo servidor substituidor e as demais
    relacionadas aos processos de usuário na ordem em que foram
    iniciados. Para sincornizar acesso a essa estrutura existe um
    semáforo. O processo que quer escrever na tabela adquire o
    semáforo, escreve e então o libera. A única sincronização
    necessária é na hora que o processo é criado e ele precisa
    adicionar uma entrada para ele na tabela. Depois disso o processo
    guarda o index referente a si na tabela. Dois processos diferentes
    terão portanto index na tabela diferente e portanto futuras
    escritas e leituras não precisam ser sincronizadas já que lidarão
    com entradas diferentes da tabela.

    Processos de usuários escrevem nessa tabela quando recebem
    indicação de pagefault do server. A entrada de page fault count no
    index do processo de alocação se refere ao número total de page
    faults e é escrita pela processo alocador. A entrada referente
    ao processo substituidor indica o número de vezes que esse
    processo entrou em ação, isto é: o número de vezes que o processo
    percebeu que haviam menos frames livres do que o estabelecido.

