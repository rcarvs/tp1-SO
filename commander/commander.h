/* 
 * File:   commander.h
 * Author: rcarvs
 *
 * Created on 2 de Outubro de 2017, 14:58
 */

#ifndef COMMANDER_H
#define COMMANDER_H

#ifdef __cplusplus
extern "C" {
#endif
    
    /*
     * Criará um pipe usando um file descriptor
     * Argumentos:
     * 0 : ponteiro para um vetor de inteiro que armazenará o resultado
     * Retorno (int):
     * 1 em caso de sucesso
     * -1 em caso de falha
     */
    int criaPipe(int*);

    /*
     * Criará um processo
     * Retorno (int):
     * Numero do processo em caso de sucesso.
     * -1 em caso de falha
     * 
     */
    int criaProcesso();
    
    /*
     * Vai ler as entradas que irão controlar a execução
     * Argumentos:
     * 0:ponteiro para o file descriptor do pipe
     */
    void comandar(int*,FILE*);
    
    /*
     * Checa se o comando da entrada ta ok
     * Argumentos:
     * 0:ponteiro para o caractere digitado pelo usuario
     * Retorno (int):
     * 1 em caso de sucesso (achou o caractere)
     * -1 em caso de falha (não achou o caractere)
     * 
     * 
     */
    int checaComando(char*);    
    

#ifdef __cplusplus
}
#endif

#endif /* COMMANDER_H */

