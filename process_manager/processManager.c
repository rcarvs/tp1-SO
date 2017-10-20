
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "processManager.h"

int debug = 1;

/*
 * A variavel quantum irá definir quantas unidades de tempo o processo irá executar antes de ser retirado da cpu
 */
int quantum = 3;

/*
 * A variavel cores irá controlar quantos processos executando ao mesmo tempo será possível.
 * int cores = 2;
 */


/*
 * A CPU será responsável por simular uma CPU 
 */
CPU cpu;

int checaComando(char *comando) {
    char alfabeto[] = {'Q', 'U', 'P', 'T'};

    for (int i = 0; i < 4; i++) {
        if (alfabeto[i] == *comando) {
            return 1;
        }
    }
    return -1;

}

void printProcesso(Processo *p, int programa) {
    printf("Processo (%d):\n", p->id);
    printf("    ID: %d\n", p->id);
    printf("    Estado: %d\n", p->estado);
    printf("    Prioridade: %d\n", p->prioridade);
    printf("    Contador de Programa: %d\n", p->PC);
    printf("    Valor: %d\n", p->var);
    printf("    PPID: %d\n", p->ppid);
    if (programa) {
        printf("    Programa: %s\n", p->programa);
    }
}

int main(int argc, char** argv) {
    //o process manager deve ficar ouvindo o commander e efetuar as ações caso receba o comando
    char *comando = (char*) malloc(sizeof (char));

    //Inicialização das listas dos processos
    TabelaPcb tabelaPcb;
    tabelaPcb.num = 0;

    EstadoPronto estadoPronto;
    estadoPronto.num = 0;

    EstadoExecutando estadoExecutando;
    estadoExecutando.num = 0;

    EstadoBloqueado estadoBloqueado;
    estadoBloqueado.num = 0;




    //abrir o primeiro processo que vai controlar toda a execução
    if (criaProcesso(getPrograma(fopen(argv[1], "r")), 0, &tabelaPcb, &estadoPronto, 0) == ERR) {
        printf("ERRO");
        return 0;
    }

    if (debug) {
        printProcesso(&tabelaPcb.processos[tabelaPcb.num - 1], 0);
    }
    //agora o primeiro processo já foi criado... a execução será controlada pelos comandos vindos do commander        
    while (1) {
        fscanf(stdin, "%c", comando);
        //fflush(stdin);
        if (checaComando(comando) == 1) {
            if (debug) {
                FILE *arquivo = fopen("arquivo.txt", "a+");
                fprintf(arquivo, "(%c)\n", *comando);
                fclose(arquivo);
            }
            switch (*comando) {
                case 'Q':
                    //fim de uma unidade de tempo. Reescalona processos e executa 
                    //vou ter sempre somente 1 processo em execução                                        
                    if (estadoExecutando.num == 0) {
                        estadoExecutando.ids = malloc(sizeof (int));
                        estadoExecutando.ids[0] = estadoPronto.ids[0];
                        estadoExecutando.num++;

                        //removo da lista de prontos
                        //arrumo os processos do estado pronto
                        estadoPronto.ids[0] = 0;
                        estadoPronto.num--;
                        for(int i=0;i<estadoPronto.num;i++){
                            estadoPronto[i] = estadoPronto[i+1];
                        }
                    }
                    escalonaProcessos(&tabelaPcb, &estadoPronto, &estadoBloqueado, &estadoExecutando);
                    executa(&tabelaPcb, &estadoExecutando);
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

void executa(TabelaPcb *tabelaPcb, EstadoExecutando *estadoExecutando,EstadoBloqueado *estadoBloqueado) {
    Processo *executante = &tabelaPcb->processos[estadoExecutando->ids[0]];
    char instrucao;
    instrucao = executante->programa[executante->PC];
    switch (instrucao) {
        case 'S':
            //atualiza valor da variavel do processo                        
            executante->PC = executante->PC + 2;
            char *valorcharzado = (char*) malloc(sizeof (char));
            int pos = 0;
            do {
                valorcharzado[pos] = executante->programa[executante->PC];
                //printf("%d",valor2);
                pos++;
                executante->PC++;
                realloc(valorcharzado, sizeof (char)*pos);
            } while (executante->programa[executante->PC] != '\n');
            int valor;
            sscanf(valorcharzado,"%d",&valor);            
            //atualizar valores dos processos
            executante->CPUInfos.quantuns_parciais++;
            executante->CPUInfos.quantuns_totais++;
            executante->var = valor;
            executante->PC++;            
            break;
        case 'A':
            //soma n na variavel do processo
            
            //fazer a mesma coisa do S mas somando
            
            printf("Reconheceu um A");
            break;
        case 'D':
            //subtrai n da variavel 
            
            //fazer a mesma coisa do S mas subtraindo 
            printf("Reconheceu um D");
                        
            
            break;
        case 'B':
            //bloqueia processo
            
            //retiro ele da lista de executando e coloco ele na lista de bloqueados
            estadoBloqueado->num++;
            realloc(estadoBloqueado->ids,sizeof(int)*estadoBloqueado->num);
            estadoBloqueado->ids[estadoBloqueado->num-1] = estadoExecutando->ids[0];            
            estadoExecutando->num--;
            executante->estado = BLOQUEADO;
            
            printf("Reconheceu um B");
            break;
        case 'E':
            //termina processo simulado
            printf("Reconheceu um E");
            break;
        case 'F':
            //cria um novo processo simulado que executa no PC+1 e espera um n que é um offset do pai
            printf("Reconheceu um F");
            break;
        case 'R':
            //substitui o programa do processo simulado e começa no pc 0
            printf("Reconheceu um R");
            break;
    }
    executante->PC++;
}

void escalonaProcessos(TabelaPcb *tabelaPcb, EstadoPronto *estadoPronto, EstadoBloqueado *estadoBloqueado, EstadoExecutando *estadoExecutando) {
    //FCFS
    Processo *executando = &tabelaPcb->processos[estadoExecutando->ids[0]];
    int substitui = 0;

    //estourou o limite de quantuns do processo?
    if (executando->CPUInfos.quantuns_parciais == quantum) {
        executando->CPUInfos->quantuns_parciais = 0;
        //esse processo deve ser substituido e outro processo escalonado no lugar dele
        //mas antes vou verificar a existencia de outro processo pra substitui-lo
        if (estadoPronto != 0) {
            //substitui o processo
            executando->estado = PRONTO;
            int idTabelaPcBExecutando = estadoExecutando->ids[0];
            estadoExecutando->ids[0] = estadoPronto->ids[0];
            for (int i = 0; i < (estadoPronto->num - 1); i++) {
                estadoPronto[i] = estadoPronto[i + 1];
            }
            estadoPronto->ids[estadoPronto->num - 1] = idTabelaPcBExecutando;
        }

    }


}


char *getPrograma(FILE *arquivo) {
    if (!arquivo) return NULL;
    char c, *programa = malloc(sizeof (char));
    int tam = 0;

    while ((c = fgetc(arquivo)) != EOF) {
        programa = realloc(programa, ++tam * sizeof (char));
        programa[tam - 1] = c;
    }
    programa[tam] = '\0';
    fclose(arquivo);
    return programa;
}

char *clonaPrograma(char *programa) {
    int i, len = strlen(programa);
    char *clone = malloc((len + 1) * sizeof (char));
    if (!clone) return NULL;
    for (i = 0; i <= len; i++)
        clone[i] = programa[i];
    return clone;
}

int criaProcesso(char *programa, int n, TabelaPcb *tabelaPcb, EstadoPronto *estadoPronto, int ppid) {
    int PC = 0;
    Processo *processo = malloc(sizeof (Processo));
    if (!processo) return ERR;
    processo->id = tabelaPcb->num; //processo recebe seu id    
    processo->ppid = ((ppid > 0) ? tabelaPcb->processos[ppid].id : 0); //processo recebe id do pai

    if (tabelaPcb->num > 0) {
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
    if (!processo->programa) return ERR;
    tabelaPcb->processos = realloc(tabelaPcb->processos, ++tabelaPcb->num * sizeof (Processo)); //aumentando tamanho da pcb
    if (!tabelaPcb->processos) return ERR;
    tabelaPcb->processos[tabelaPcb->num - 1] = *processo; //pcb recebe novo processo

    if (estadoPronto->num == 0) {
        estadoPronto->ids = malloc(sizeof (int));
    } else {
        estadoPronto->ids = realloc(estadoPronto->ids, ++estadoPronto->num * sizeof (int));
    }

    estadoPronto->ids[estadoPronto->num - 1] = processo->id; //tabela de estado pronto recebe o processo criado

    if (!estadoPronto->ids) return ERR;
    return 1; //terminou com sucesso
}
/*
void substituiProcesso();
void alteraEstadoProcesso();
void alteraContextoProcesso();

void chamaReporter();
 */
