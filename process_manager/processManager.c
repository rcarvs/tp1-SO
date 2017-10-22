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


/*
 * A variável escalonador define qual o escalonador a ser utilizado na execução do programa
 */
int escalonador;

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
    printf("    Quatuns Parciais: %d\n", p->CPUInfos.quantuns_parciais);
    printf("    Quatuns Totais: %d\n", p->CPUInfos.quantuns_totais);
    if (programa) {
        printf("    Programa:\n       ");
        for (int i = 0; p->programa[i] != '\0'; i++) {
            if (p->programa[i] == '\n') {
                printf("\n       ");
                continue;
            }
            printf("%c", p->programa[i]);
        }
        //printf("    Programa: %s\n", p->programa);
    }
    printf("\n\n");
}

void printTabela(TabelaPcb *tabelaPcb, int *ids, int size, char *nomeEstado) {
    printf("ESTADO %s\n", nomeEstado);
    for (int i = 0; i < size; i++) {
        printProcesso(&tabelaPcb->processos[ids[i]], 1);
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

    /*Definindo o escalonador a ser utilizado
     *Macros: ESCAL_RR, ESCAL_LOTERIA
     */
    escalonador = ESCAL_RR;

    //abrir o primeiro processo que vai controlar toda a execução
    if (criaProcesso(getPrograma(fopen(argv[1], "r")), 0, &tabelaPcb, &estadoPronto, 0) == ERR) {
        printf("ERRO");
        return 0;
    }

    if (debug) {
        //printProcesso(&tabelaPcb.processos[tabelaPcb.num - 1], 1);
    }

    int print = 0;

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
                    print = 1;
                    //fim de uma unidade de tempo. Reescalona processos e executa
                    //vou ter sempre somente 1 processo em execução
                    if (estadoExecutando.num == 0 && estadoPronto.num > 0) {
                        estadoExecutando.ids = malloc(sizeof (int));
                        estadoExecutando.ids[0] = estadoPronto.ids[0];
                        estadoExecutando.num++;

                        //removo da lista de prontos
                        //arrumo os processos do estado pronto
                        estadoPronto.ids[0] = 0;
                        estadoPronto.num--;
                        for (int i = 0; i < estadoPronto.num; i++) {
                            estadoPronto.ids[i] = estadoPronto.ids[i + 1];
                        }
                    }

                    if (estadoExecutando.num > 0) {
                        //só faço isso se consegui colocar um processo aqui, senao ta tudo bloqueado e nao tem pq fazer isso
                        escalonaProcessos(&tabelaPcb, &estadoPronto, &estadoBloqueado, &estadoExecutando);
                        executa(&tabelaPcb, &estadoExecutando, &estadoPronto, &estadoBloqueado);
                    }


                    break;
                case 'U':
                    print = 1;
                    //Desbloqueie o primeiro processo simulado que está na fila de bloqueados.
                    if (estadoBloqueado.num > 0) {
                        //tem algum processo bloqueado para ser desbloqueado
                        tabelaPcb.processos[estadoBloqueado.ids[0]].estado = PRONTO;
                        estadoBloqueado.num--;
                        estadoPronto.num++;
                        estadoPronto.ids = realloc(estadoPronto.ids, estadoPronto.num * sizeof (int));
                        estadoPronto.ids[estadoPronto.num - 1] = estadoBloqueado.ids[0];
                        estadoBloqueado.ids[0] = 0;
                        for (int i = 0; i < estadoBloqueado.num - 1; i++) {
                            estadoBloqueado.ids[i] = estadoBloqueado.ids[i - 1];
                        }
                        if (estadoBloqueado.num > 0) {
                            estadoBloqueado.ids = realloc(estadoBloqueado.ids, sizeof (int)*estadoBloqueado.num);
                        }
                        tabelaPcb.processos[estadoBloqueado.ids[0]].estado = PRONTO;
                    }
                    break;
                case 'P':
                    print = 1;
                    //Imprima o estado atual do sistema
                    break;
                case 'T':
                    print = 1;
                    //Imprima o estado atual do sistema
                    break;
                default:
                    break;
            }

            if (*comando == 'T') {
                print = 1;
                break;
            }
            if (print) {
                //system("clear");
                printTabela(&tabelaPcb, estadoExecutando.ids, estadoExecutando.num, "EXECUTANDO");
                printf("\n");
                printTabela(&tabelaPcb, estadoPronto.ids, estadoPronto.num, "PRONTO");
                printf("\n");
                printTabela(&tabelaPcb, estadoBloqueado.ids, estadoBloqueado.num, "BLOQUEADO");
                printf("\n\n ---------------- \n\n");
            }
        }
        //fflush(stdout);
        //write(1,&retorno,sizeof(int));


        //*comando = 'A';
        //fprintf(stdout, "%c", *comando);


    }
    return EXIT_SUCCESS;
}

void executa(TabelaPcb *tabelaPcb, EstadoExecutando *estadoExecutando, EstadoPronto *estadoPronto, EstadoBloqueado *estadoBloqueado) {
    Processo *executante = &tabelaPcb->processos[estadoExecutando->ids[0]];
    char instrucao, *file, *path = "../input/";
    int soma = 0, n;
    instrucao = executante->programa[executante->PC];
    printf("3 - Contador de programa: %c\n",executante->programa[executante->PC]);
    switch (instrucao) {
        case 'S':
            //atualiza valor da variavel do processo
            //atualizar valores dos processos

            sscanf(&executante->programa[executante->PC], "S %d", &executante->var);
            for (; (executante->programa[executante->PC] != '\n') && (executante->programa[executante->PC] != '\0') ? 1 : (executante->PC++ * 0); executante->PC++);
            if (debug) {
                printf("\n### Reconheceu um S ###\n");
            }
            break;
        case 'A':
            //incrementa n na variavel do processo

            sscanf(&executante->programa[executante->PC], "A %d", &soma);
            executante->var += soma;
            //Avança pc para a próxima instrução
            for (; (executante->programa[executante->PC] != '\n') && (executante->programa[executante->PC] != '\0') ? 1 : (executante->PC++ * 0); executante->PC++);
            if (debug) {
                printf("\n### Reconheceu um A ###\n");
            }

            break;
        case 'D':

            sscanf(&executante->programa[executante->PC], "D %d", &soma);
            executante->var -= soma;
            //Avança pc para a próxima instrução
            for (; (executante->programa[executante->PC] != '\n') && (executante->programa[executante->PC] != '\0') ? 1 : (executante->PC++ * 0); executante->PC++);
            //subtrai n da variavel

            //fazer a mesma coisa do S mas subtraindo
            if (debug) {
                printf("\n### Reconheceu um D ###\n");
            }


            break;
        case 'B':
            //bloqueia processo
            //retiro ele da lista de executando e coloco ele na lista de bloqueados
            estadoBloqueado->num++;
            if ((estadoBloqueado->num - 1) == 0) {
                estadoBloqueado->ids = malloc(sizeof (int));
            } else {
                estadoBloqueado->ids = realloc(estadoBloqueado->ids, sizeof (int)*estadoBloqueado->num);
            }
            estadoBloqueado->ids[estadoBloqueado->num - 1] = estadoExecutando->ids[0];
            for (int i = 0; i < estadoExecutando->num - 1; i++)
                estadoExecutando->ids[i] = estadoExecutando->ids[i + 1];
            estadoExecutando->num--;
            estadoExecutando->ids = realloc(estadoExecutando->ids, estadoExecutando->num * sizeof (int));
            executante->estado = BLOQUEADO;
            for (; (executante->programa[executante->PC] != '\n') && (executante->programa[executante->PC] != '\0') ? 1 : (executante->PC++ * 0); executante->PC++);
            if (debug) {
                printf("\n### Reconheceu um B ###\n");
            }

            break;
        case 'E':
            //termina processo simulado
            //REMOVER ELE DA LISTA DE EXECUTANDO E DA TABELA PCB
            for (int i = estadoExecutando->ids[0]; i < (tabelaPcb->num - 1); i++) {
                tabelaPcb->processos[i] = tabelaPcb->processos[i + 1];
            }
            tabelaPcb->num--;
            tabelaPcb->processos = realloc(tabelaPcb->processos, sizeof (int)*tabelaPcb->num);
            estadoExecutando->ids[0] = NULL;
            estadoExecutando->num--;

            //Agora preciso colocar ele numa lista de processos finalizados pra poder tirar a média



            if (debug) {
                printf("\n### Reconheceu um E ###\n");
            }





            break;
        case 'F':
            sscanf(&executante->programa[executante->PC], "F %d", &n);
            printf("Contador de programa: %d\n",executante->PC);
            for (; (executante->programa[executante->PC] != '\n') && (executante->programa[executante->PC] != '\0') ? 1 : (executante->PC++ * 0); executante->PC++);
            printf("Contador de programa: %d e valor de n eh %d\n",executante->PC,n);
            if (!criaProcesso(executante->programa, executante->PC, tabelaPcb, estadoPronto, executante->id)) {
                printf("Erro\n");
                return;
            }

            //agora desloco o programa atual n linhas
            for (int i = 0; i < n; i++) {
                printf("1 - Contador de programa: %d\n",executante->PC);
                for (; (executante->programa[executante->PC] != '\n') && (executante->programa[executante->PC] != '\0') ? 1 : (executante->PC++ * 0); executante->PC++);
                printf("2 - Contador de programa: %d\n",executante->PC);
            }
            printf("3 - Contador de programa: %d\n",executante->PC);
            //cria um novo processo simulado que executa no PC+1 e espera um n que é um offset do pai
            if (debug) {
                printf("\n### Reconheceu um F ###\n");
            }

            break;
        case 'R':
            sscanf(&executante->programa[executante->PC], "R %s", file);
            path = realloc(path, (strlen(file) + strlen("../input/")) * sizeof(char));
            strcat(path, file);
            executante->programa = getPrograma(fopen(path, "r"));
            executante->PC = 0;
            free(path);
            free(file);
            printf("\n### Reconheceu um R ###\n");
            break;
    }
    //executante->PC++;
}

void escalonaProcessos(TabelaPcb *tabelaPcb, EstadoPronto *estadoPronto, EstadoBloqueado *estadoBloqueado, EstadoExecutando *estadoExecutando) {
    //round-robin
    Processo *executando = &tabelaPcb->processos[estadoExecutando->ids[0]];
    executando->CPUInfos.quantuns_parciais++;
    executando->CPUInfos.quantuns_totais++;
    int sorteado, idTabelaPcBExecutando;

    switch (escalonador) {
        case ESCAL_RR:
            //estourou o limite de quantuns do processo?
            if (executando->CPUInfos.quantuns_parciais == quantum) {
                executando->CPUInfos.quantuns_parciais = 0;
                //esse processo deve ser substituido e outro processo escalonado no lugar dele
                //mas antes vou verificar a existencia de outro processo pra substitui-lo
                printf("Vai verificar estadoPronto com %d", estadoPronto->num);
                if (estadoPronto->num != 0) {
                    //substitui o processo
                    executando->estado = PRONTO;
                    idTabelaPcBExecutando = estadoExecutando->ids[0];
                    estadoExecutando->ids[0] = estadoPronto->ids[0];
                    for (int i = 0; i < (estadoPronto->num - 1); i++) {
                        estadoPronto->ids[i] = estadoPronto->ids[i + 1];
                    }
                    estadoPronto->ids[estadoPronto->num - 1] = idTabelaPcBExecutando;
                }


            }
            break;

        case ESCAL_LOTERIA:
            sorteado = rand() % (estadoPronto->num - 1);
            idTabelaPcBExecutando = estadoExecutando->ids[0];
            estadoExecutando->ids[0] = estadoPronto->ids[sorteado];
            for (int i = 0; i < (estadoPronto->num - 1); i++) {
                estadoPronto->ids[i] = estadoPronto->ids[i + 1];
            }
            estadoPronto->ids[estadoPronto->num - 1] = idTabelaPcBExecutando;
            break;

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
    int PC = n;
    Processo *processo = malloc(sizeof (Processo));
    if (!processo) return ERR;
    processo->id = tabelaPcb->num; //processo recebe seu id
    processo->ppid = ((ppid > 0) ? tabelaPcb->processos[ppid].id : 0); //processo recebe id do pai

    /*if (tabelaPcb->num > 0) {
        PC = tabelaPcb->processos[ppid].PC + 1; //processo filho começa uma instrução abaixo do pai
        tabelaPcb->processos[ppid].PC += n; //processo pai continua n instruções a frente
    }*/

    processo->PC = PC; //processo recebe valor de seu contador de programa
    processo->estado = PRONTO; //estado do processo é pronto de início
    processo->var = 0; //variável inteira recebe zero

    /* o processo recebe seu programa
     * o programa do processo pai é clonado pois existe o risco do processo filho permanecer sem intruções
     * em caso de término do processo pai.
     */
    processo->programa = clonaPrograma(programa);
    if (!processo->programa) return ERR;

    if (tabelaPcb->num == 0) {
        tabelaPcb->processos = malloc(sizeof (Processo));
        tabelaPcb->num++;
    } else {
        tabelaPcb->processos = realloc(tabelaPcb->processos, ++tabelaPcb->num * sizeof (Processo)); //aumentando tamanho da pcb
    }

    if (!tabelaPcb->processos) return ERR;
    tabelaPcb->processos[tabelaPcb->num - 1] = *processo; //pcb recebe novo processo
    if (estadoPronto->num <= 0) {
        estadoPronto->ids = malloc(sizeof (int));
        estadoPronto->num++;
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
