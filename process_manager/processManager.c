
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "processManager.h"

int debug = 1;

int checaComando(char *comando) {
    char alfabeto[] = {'Q', 'U', 'P', 'T'};

    for (int i = 0; i < 4; i++) {
        if (alfabeto[i] == *comando) {
            return 1;
        }
    }
    return -1;

}

int main(int argc, char** argv) {
    //o process manager deve ficar ouvindo o commander e efetuar as ações caso receba o comando
    char *comando = (char*) malloc(sizeof (char));
    int retorno = 1;
    while (1) {

        fscanf(stdin, "%c", comando);
        //fflush(stdin);        
        if (checaComando(comando) == 1) {
            if (debug) {
                FILE *arquivo = fopen("arquivo.txt", "a+");
                fprintf(arquivo, "(%c)\n", *comando);
                fclose(arquivo);
            }
            switch(*comando){
                case 'Q':
                    //fim de uma unidade de tempo
                    break;
                case 'U':
                    //Desbloqueie o primeiro processo simulado que está na fila de bloqueados.                    
                    break;
                case 'P':
                    //Imprima o estado atual do sistema
                    break;
                case 'T':
                    //Imprima o estado atual do sistema
                    break;
                default:
                    break;
            }
            
            if (*comando == 'T') {

                break;
            }
        }
        //fflush(stdout);
        //write(1,&retorno,sizeof(int));


        //*comando = 'A';
        //fprintf(stdout, "%c", *comando);


    }


    return EXIT_SUCCESS;
}


void criaProcesso();
void substituiProcesso();
void alteraEstadoProcesso();
void escalonaProcessos();
void alteraContextoProcesso();

void chamaReporter();


