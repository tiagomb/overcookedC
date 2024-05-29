#include "libpilha.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

//Cria um nó que contém o dado passado pelo usuário. Caso a alocação falhe, o programa sai com um código de erro
struct nop *criaNop (char dado){
    struct nop *p;
    p = malloc(sizeof (struct nop));
    if (!p){
        printw ("Falha ao alocar memória!\n");
        exit (1);
    }
    p->dado = dado;
    p->prox = NULL;
    return p;
}

/*Retira o elemento que está no topo da pilha*/
struct nop *pop (struct pilha **pilha){
    struct nop *aux;
    aux = (*pilha)->topo;
    (*pilha)->topo = (*pilha)->topo->prox;
    (*pilha)->tam--;
    return aux;
}

/*Cria um nó e o adiciona no topo da pilha, caso a pilha não esteja do tamanho máximo*/
int push (struct pilha **pilha, char dado){
    struct nop *aux;
    if ((*pilha)->tam < (*pilha)->max){
        aux = criaNop(dado);
        if (!(*pilha)->tam)
            (*pilha)->topo = aux;
        else{
            aux->prox = (*pilha)->topo;
            (*pilha)->topo = aux;
        }
        (*pilha)->tam++;
        return 0;
    }
    return 1;
}

/*Mostra o elemento do topo*/
void mostraTopo (struct nop *topo){
    printw ("Topo da pilha: %c\n", topo->dado);
}

/*A função retorna 1 caso a pilha esteja vazia e 0 caso não esteja*/
int estaVaziaPilha (struct pilha *pilha){
    if (!pilha->tam)
        return 1;
    return 0;
}

/*Realiza a alocação da pilha e inicializa as variáveis. Caso a alocação falhe, o programa sai com um código de erro*/
int inicializaPilha (struct pilha **pilha){
    (*pilha) = malloc (sizeof (struct pilha));
    if (*pilha){
        (*pilha)->max = 10;
        (*pilha)->tam = 0;
        (*pilha)->topo = NULL;
        return 0;
    }
    printw ("Falha ao alocar memória!\n");
    exit (1);
}

/*Essa função recursiva imprime os elementos da pilha na ordem em que foram adicionados (do primeiro para o último)*/
void imprimeRefeicao (struct nop *topo){
    if(!topo) 
        return;
    imprimeRefeicao(topo->prox);
    printw("[%c]", topo->dado);
}

/*Destrói a pilha, dando free tem todos os nós e por fim na struct pilha em si*/
void destroiPilha (struct pilha **pilha){
    struct nop *aux;
    while ((*pilha)->topo){
        aux = (*pilha)->topo;
        (*pilha)->topo = (*pilha)->topo->prox;
        aux->dado = 0;
        free (aux);
        aux = NULL;
    }
    (*pilha)->tam = 0;
    (*pilha)->max = 0;
    free (*pilha);
    *pilha = NULL;
}