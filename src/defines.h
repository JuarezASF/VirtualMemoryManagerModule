//
// Created by jasf on 5/14/16.
//

#ifndef VIRTUALMEMORYMANAGER_DEFINES_H
#define VIRTUALMEMORYMANAGER_DEFINES_H

//----------------------------
// configura servidor de paginação e
//----------------------------
/**
 * Número de frames na tabela de paginação. Define o tamanho da tabela.
 */
#define NUMERO_FRAMES 5

//----------------------------
//configura servidor de substituição
//----------------------------
/**
 * Os valores tem o significado descrito na proposta do trabalho.
 *
 * Notar que:
 *
 *     o servidor é ativado caso o número de frames ocupados seja MAIOR QUE(>) MAX_OCUPACAO
 *     o servidor livra frames enquanto o número de frames ocupados for MAIOR QUE(>) OCUPACAO_OK
 */
#define MAX_OCUPACAO 4
#define OCUPACAO_OK 3
/**
 * O servidor de substituição verifica a utilização da tabela de tempos em tempos.
 * Configure esse tempo aqui
 */
#define PAGE_SUBS_SLEEP_TIME 1e5

//----------------------------
//configura processos usuários
//----------------------------
/**
 * O processo usuário dorme entre a referência a uma página e a próxima referência. Configure
 * esse tempo aqui.
 * Para observar o funcionamento do código sete para um valor que te permita acompanhar o que
 * está acontecendo.
 */
#define USER_USLEEP_TIPE 5e5 //in unites of microsecond [1 usec = 10^-6 sec]

//----------------------------
//Configura tabela de PIDs
//----------------------------
/**
 * A tabela precisa de um tamanho máximo de entradas já que utiliza memória compartilhada
 * e para isso precisamos declarar um tamanho em tempo de criação
 */
#define MAX_NUMERO_PROCESSOS 1024


#endif //VIRTUALMEMORYMANAGER_DEFINES_H
