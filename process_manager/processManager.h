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

typedef struct Processo{
    int id;
    
} Processo;    
    
typedef struct Tempo;
typedef struct TabelaPcb;
typedef struct EstadoPronto;
typedef struct EstadoBloqueado;
typedef struct EstadoExecutando;

    
typedef struct CPU{
    int contador;
    int valor;
} CPU;   
    
void criaProcesso(FILE*);

#ifdef __cplusplus
}
#endif

#endif /* PROCESSMANAGER_H */

