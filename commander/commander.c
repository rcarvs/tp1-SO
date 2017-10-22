/* 
 * File:   commander.c
 * Author: rcarvs
 *
 * Created on 2 de Outubro de 2017, 14:57
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include "commander.h"

int debug = 1;

int criaPipe(int *fd) {
    //fd = (int*) malloc(sizeof (int)*2);
    if (pipe(fd) < 0) {
        return -1;
    }
    return 1;
}

int criaProcesso() {
    int pid;
    if ((pid = fork()) < 0) {
        return -1;
    }
    return pid;
}



void comandar(int *fd, FILE *entrada) {
    //essa função será responsável por passar os comandos do commander para o process manager    
    //ler 1 comando por segundo    
    
    
    //o primeiro passo aqui vai ser enviar o stdout para o proccess manager para que ele consiga imprimir as coisas na saída padrão
    

    
    char *comando = (char*) malloc(sizeof (char)*2);
    int retorno;
    while ((*comando = fgetc(entrada)) != EOF) {
        clock_t start = clock();        
        write(fd[1], comando, sizeof (char));
        if (debug) {
            printf("Enviou o comando %c \n", *comando);
        }

        /* esperar a resposta do proccess manager
         * a resposta do proccess manager será 0 ou 1
         * se for 0 deu erro
         * se for 1 deu tudo certo
        */                
        
        //read(fd[0],&retorno,sizeof(int));
        
        //printf("Recebeu o retorno do proccess manager: %d\n",retorno);
        
        fgetc(entrada); //pular /n do arquivo de entrada
        clock_t end = clock();
        if (((end - start) / CLOCKS_PER_SEC) < 1) {
            usleep((1 - (((end - start) / CLOCKS_PER_SEC)))*1000000);
        }
    }
    //imprime estado atual do sistema 
    printf("ESTADO ATUAL DO SISTEMA DEVE VIR DAQUI\n");
}

int main(int argc, char** argv) {
    //o fd será o file descriptor do pipe
    int fd[2];        
    //cria o pipe que será responsável por comunicar com o process manager
    if (criaPipe(fd) == -1) {
        printf("Ocorreu um erro ao criar o pipe.\n");
        exit(1);
    }
    //cria um novo processo que irá executar a partir da linha abaixo.
    int pid = criaProcesso();
    if (pid == -1) {
        printf("Ocorreu um erro ao criar o novo processo (process manager)\n");
        exit(1);
    }

    if (pid == 0) {
        //sou o filho (proccess amanger)

        dup2(fd[0], 0); //dup2 faz o close e o dup de forma atomica
        dup2(fd[1], 1);
        //checar se o executavel do process manager existe
        execvp("./bin/process_manager", argv + 1);
        perror("Erro ao inicializar o executável process_manager");
        exit(1);
    } else {
        comandar(fd, fopen("./input/commanderIn.txt", "r"));
    }

    //parte comum a ambos os processos caso nenhum seja finalizado
    return EXIT_SUCCESS;
}

