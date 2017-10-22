

#ifndef REPORTER_H
#define REPORTER_H

#ifdef __cplusplus
extern "C" {
#endif

/*
     * imprimira na saida padrao o reporter
     * Argumentos:
     * Retorno (void):
     */
void imprimeEstadoAtual();

#ifdef __cplusplus
}
#endif


    typedef struct ProcessoCPUInfos
{
  int quantuns_totais;
  int quantuns_parciais;
} ProcessoCPUInfos;

typedef struct Processo
{
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
typedef struct TabelaPcb
{
  Processo *processos;
  int num;
} TabelaPcb;

typedef struct EstadoPronto
{
  int *ids;
  int num;
} EstadoPronto;

typedef struct EstadoBloqueado
{
  int *ids;
  int num;
} EstadoBloqueado;

typedef struct EstadoExecutando
{
  int *ids;
  int num;
} EstadoExecutando;

typedef struct CPU
{
  int contador;
  int valor;
} CPU;

void imprimeEstadoAtual(TabelaPcb *tabelaPcb, EstadoExecutando *estadoExecutando, EstadoPronto *estadoPronto, EstadoBloqueado *estadoBloqueado);

void printEstadoExecutando(EstadoExecutando *estadoExecutando, TabelaPcb *tabelaPcb);

void printEstadoPronto(EstadoPronto *estadoPronto, TabelaPcb *tabelaPcb);

void printEstadoBloqueado(EstadoBloqueado *estadoBloqueado, TabelaPcb *tabelaPcb);

int tempototal(TabelaPcb *tabelaPcb);

#endif 
