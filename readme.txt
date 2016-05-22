### Universidade de Brasília
##### Sistemas Operacionais- 1/2016
##### Professora: Alba

Alunos

Gabriel Franklin Braz de Medeiros 15/0126387    gabriel.medeiros93@gmail.com

Juarez Aires Sampaio Filho      11/0032829      juarez.asf@gmail.com


## Trabalho 1 - Gerente de Memória Virtual

1.1 Requisitos


2. Instruções para compilação

2.1 Compilação com cmake

    mkdir build
    cd build
    cmake ..
    make


2.2 Por linha de comando

//TODO

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
     ./runAll

O programa 'userProcess' lê dos argumentos passado para o programa o nome do arquivo contem as páginas
a serem referenciadas

3.1 Ordem de execução dos Programas

4. Descrição da comunicação entre os processos.

4.1 Procusso usuário <-> servidor de alocação de mensagem

    Os processos de usuário mandam pedidos de alocação por meio de uma fila de mensagem. A mensagem
que o processo de usuário envia contém o PID do processo requerente. Para um dado pedido, o servdior
coloca uma mensagem em uma segunda fila de mensagens. Essa mensagem de resposta possui um identificador
de mensagem igual a PID do processo a que se destina. A mensagem de resposta também indica se o
servidor de memória passou por um pagefault durante aquele pedido. O servidor de alocação de páginas
lê a fila de entrada sequencialmente.

4.2 servidor de alocação de mensagem  <-> servidor de substituição de páginas

    Ambos os servidores manipulam uma tabela contendo informação dos frames. Essa tabela está
em memória compartilhada. Para sincronizar o acesso a memória existe um semáforo. O processo que
quer acessar a tabela ( para leitura ou para escrita ) adquire o semáforo e faz as operações que
são necessárias. Quando não precisa mais da tabela, o processo libera o semáforo.


4.2 Servidor de substituição

    O processo de substituição é ativado periodicamente. Para isso a chamda de sistema sleep é
    utilizada.

4.3 Processo de shutdown <-> outros processos

    Cada processo registra em seu construtor uma resposta ao sinal SIGUSR2. Ao receber esse sinal
os processos de usuário, servidor de alocação e substituição de páginas encerram suas atividades
imediatamente por meio da chamada exit.

