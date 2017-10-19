
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
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

    TabelaPcb tabelaPcb;
    EstadoPronto estadoPronto;
    tabelaPcb.num = 0;
    estadoPronto.num = 0;
    //abrir o primeiro processo que vai controlar toda a execução
    if(criaProcesso(getPrograma(fopen(argv[1], "r")), 0, &tabelaPcb, &estadoPronto, 0) == ERR){
        printf("ERRO");
        return 0; 
    }
    printf("Processo:\n%d\n%d\n%d\n%d\n%d\n%s", tabelaPcb.processos[0].id, tabelaPcb.processos[0].ppid, tabelaPcb.processos[0].PC, tabelaPcb.processos[0].estado, tabelaPcb.processos[0].var, tabelaPcb.processos[0].programa);
    criaProcesso(tabelaPcb.processos[0].programa, 3, &tabelaPcb, &estadoPronto, 0);
    printf("\nProcesso:\n%d\n%d\n%d\n%d\n%d\n%s", tabelaPcb.processos[1].id, tabelaPcb.processos[1].ppid, tabelaPcb.processos[1].PC, tabelaPcb.processos[1].estado, tabelaPcb.processos[1].var, tabelaPcb.processos[1].programa);
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

char *getPrograma(FILE *arquivo){
    if(!arquivo) return NULL;
    char c, *programa = malloc(sizeof(char));
    int tam = 0;

    while((c = fgetc(arquivo)) != EOF){
        programa = realloc(programa, ++tam * sizeof(char));
        programa[tam - 1] = c;
    }
    programa[tam] = '\0';
    fclose(arquivo);
    return programa;
}

char *clonaPrograma(char *programa){ 
    int i, len = strlen(programa);
    char *clone = malloc((len + 1) * sizeof(char));
    if(!clone) return NULL;
    for(i = 0; i <= len; i++)
        clone[i] = programa[i];
    return clone;
}

int criaProcesso(char *programa, int n, TabelaPcb *tabelaPcb, EstadoPronto *estadoPronto, int ppid){
    int PC = 0;
    Processo *processo = malloc(sizeof(Processo));
    if(!processo) return ERR;
    processo->id = tabelaPcb->num; //processo recebe seu id
    processo->ppid = ppid; //processo recebe id do pai

    if(tabelaPcb->num > 0){
        PC = tabelaPcb->processos[ppid].PC + 1; //processo filho começa uma instrução abaixo do pai
        tabelaPcb->processos[ppid].PC += n; //processo pai continua n instruções a frente
    }

    processo->PC = PC; //processo recebe valor de seu contador de programa
    processo->estado = PRONTO; //estado do processo é pronto de início
    processo->var = 0; //variável inteira recebe zero

    /* o processo recebe seu programa
    * o programa do processo pai é clonado pois existe o risco do processo filho permanecer sem intruções
    * em caso de término do processo pai.
    */
    processo->programa = clonaPrograma(programa);
    if(!processo->programa) return ERR;
    tabelaPcb->processos = realloc(tabelaPcb->processos, ++tabelaPcb->num * sizeof(Processo)); //aumentando tamanho da pcb
    if(!tabelaPcb->processos) return ERR;
    tabelaPcb->processos[tabelaPcb->num - 1] = *processo; //pcb recebe novo processo

    if(estadoPronto->num == 0){
        estadoPronto->ids = malloc(sizeof(int));
    }else{
        estadoPronto->ids = realloc(estadoPronto->ids, ++estadoPronto->num * sizeof(int));
    }

    estadoPronto->ids[estadoPronto->num - 1] = processo->id; //tabela de estado pronto recebe o processo criado

    if(!estadoPronto->ids) return ERR;
    return 1; //terminou com sucesso
}

void substituiProcesso();
void alteraEstadoProcesso();
void escalonaProcessos();
void alteraContextoProcesso();

void chamaReporter();
