/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   processManager.h
 * Author: rcarvs
 *
 * Created on 2 de Outubro de 2017, 16:13
 */

#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

#define PRONTO 0
#define EXECUTANDO 1
#define BLOQUEADO 2
#define ERR -1
#define ESCAL_RR -3
#define ESCAL_LOTERIA -2

/*
 * Struct para guardar informações do processo referentes à suas execuções
 */
typedef struct ProcessoCPUInfos{
    int quantuns_totais;
    int quantuns_parciais;
} ProcessoCPUInfos;

typedef struct Processo{
    int id;
    int ppid;
    int PC;
    int estado;
    int var;
    int prioridade;
    char *programa;
    ProcessoCPUInfos CPUInfos;
} Processo;

typedef struct Tempo;
typedef struct TabelaPcb{
	Processo *processos;
	int num;
} TabelaPcb;

typedef struct EstadoPronto{
	int *ids;
	int num;
} EstadoPronto;

typedef struct EstadoBloqueado{
	int *ids;
	int num;
} EstadoBloqueado;

typedef struct EstadoExecutando{
	int *ids;
	int num;
} EstadoExecutando;

typedef struct Finalizados{
    Processo *processos;
    int num;
} Finalizados;

typedef struct CPU{
    int contador;
    int valor;
} CPU;

char *getPrograma(FILE *);
char *clonaPrograma(char *);
int getValorCharzado(Processo *);
void executa(TabelaPcb *, EstadoExecutando *, EstadoPronto *, EstadoBloqueado *);
/*
 * Função responsável por criar um novo processo e colocalo na tabela pcb
 * Parametros:
 * 0 -> Instruções do novo processo
 * 1 -> offset de instruções do novo processo
 * 2 -> tabela pcb para inserir o processo
 * 3 -> tabela de processos prontos
 * 4 -> id do processo pai na tabela pcb
 */
int criaProcesso(char *, int, TabelaPcb *, EstadoPronto *, int);

void escalonaProcessos(TabelaPcb *, EstadoPronto *, EstadoBloqueado *, EstadoExecutando *);

#ifdef __cplusplus
}
#endif

#endif /* PROCESSMANAGER_H */
