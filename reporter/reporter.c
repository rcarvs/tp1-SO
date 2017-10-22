#include <stdio.h>
#include <stdlib.h>
#include "reporter.h"

int main(int argc, char** argv) {
    
}

void imprimeEstadoAtual(TabelaPcb *tabelaPcb, EstadoExecutando *estadoExecutando, EstadoPronto *estadoPronto, EstadoBloqueado *estadoBloqueado){
    
    printf("***********************************\n");
    printf("Estado de Sistema \n");
    printf("***********************************\n");

    printf("TEMPO ATUAL: %d \n", tempototal(tabelaPcb) );
    printf("PROCESSO EXECUTANDO: \n");
    printf("pid, ppid, prioridade, valor, tempo inicio, CPU usada ate agora\n");
    printEstadoExecutando(estadoExecutando,tabelaPcb);
    printf("\n");
    
    printf("BLOQUEADO: \n");
    printf("fila de processos bloqueados: \n");
    printf("pid, ppid, prioridade, valor, tempo inicio, CPU usada ate agora \n");
    printEstadoBloqueado(estadoBloqueado,tabelaPcb);
    //prcessos da lista de bloqueados
    printf("pid, ppid, prioridade, valor, tempo inicio, CPU usada ate agora \n");
    printf("\n");

    printf("PROCESSOS PRONTOS: \n");
    //processos da lista de prontos
    printf("pid, ppid, prioridade, valor, tempo inicio, CPU usada ate agora");
    printEstadoPronto(estadoPronto, tabelaPcb);
    printf("\n");
    printf("***********************************\n");
    
    

}

void printEstadoExecutando(EstadoExecutando *estadoExecutando, TabelaPcb *tabelaPcb){
    
    int posicaoProcesso;
    for(int i = 0 ; i < estadoExecutando->num ; i++){
        posicaoProcesso = pesquisaIndiceVetor(estadoExecutando->ids[i], tabelaPcb);
        printf("%d, %d, %d, %d, %d, %d\n",tabelaPcb->processos[posicaoProcesso].id, tabelaPcb->processos[posicaoProcesso].ppid, tabelaPcb->processos[posicaoProcesso].prioridade, tabelaPcb->processos[posicaoProcesso].var, tabelaPcb->processos[posicaoProcesso].CPUInfos.quantuns_parciais,tempototal(tabelaPcb));
    }
}

void printEstadoPronto(EstadoPronto *estadoPronto, TabelaPcb *tabelaPcb){
    
    int posicaoProcesso;
    for(int i = 0 ; i < estadoPronto->num ; i++){
        posicaoProcesso = pesquisaIndiceVetor(estadoPronto->ids[i], tabelaPcb);
        printf("%d, %d, %d, %d, %d, %d\n",tabelaPcb->processos[posicaoProcesso].id, tabelaPcb->processos[posicaoProcesso].ppid, tabelaPcb->processos[posicaoProcesso].prioridade, tabelaPcb->processos[posicaoProcesso].var, tabelaPcb->processos[posicaoProcesso].CPUInfos.quantuns_parciais, tempototal(tabelaPcb));
    }
}

void printEstadoBloqueado(EstadoBloqueado *estadoBloqueado, TabelaPcb *tabelaPcb){
    
    int posicaoProcesso;
    for(int i = 0 ; i < estadoBloqueado->num ; i++){
        posicaoProcesso = pesquisaIndiceVetor(estadoBloqueado->ids[i], tabelaPcb);
        printf("%d, %d, %d, %d, %d, %d\n",tabelaPcb->processos[posicaoProcesso].id, tabelaPcb->processos[posicaoProcesso].ppid, tabelaPcb->processos[posicaoProcesso].prioridade, tabelaPcb->processos[posicaoProcesso].var, tabelaPcb->processos[posicaoProcesso].CPUInfos.quantuns_parciais, tempototal(tabelaPcb));
    }
}




int pesquisaIndiceVetor(int ID, TabelaPcb *tabelaPcb){

    for(int i = 0 ; i < tabelaPcb->num ; i++){
        if( ID == tabelaPcb->processos[i].id){
            return i;
        }
    }
}

//calcula o tempo total em quantuns de todos os processos que estão executando
int tempototal(TabelaPcb *tabelaPcb){
    int total = 0;
    for(int i = 0 ; i < tabelaPcb->num ; i ++){
        total += tabelaPcb->processos[i].CPUInfos.quantuns_totais;
    }
    return total;
}

int main(int argc, char** argv) {
    TabelaPcb *tabelaPcb = (TabelaPcb*) malloc(sizeof(TabelaPcb));
    imprimeEstadoAtual(tabelaPcb,estadoExecutando,estadoPronto,estadoBloqueado);
    
    

    return 0;
}
