#include "libfila.h"
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

/*Cria um nó para a fila que contém o número da refeição passado por outra função e uma pilha com os ingredientes da refeição correspondente*/
struct nof *criaNof (int cliente, int refeicao){
    struct nof *f;
    f = malloc(sizeof (struct nof));
    if (!f){ //caso a alocação falhe o programa sai com um código de erro
        printw ("Falha ao alocar memória!\n");
        exit (1);
    }
    f->cliente = cliente;
    f->numRef = refeicao;
    inicializaPilha (&f->refeicao);
    if (!f->refeicao){ //caso a alocação falhe o programa sai com um código de erro
        printw ("Falha ao alocar memória!\n");
        exit (1);
    }
    switch (refeicao){
        case 0:
            push (&f->refeicao, 'p');
            push (&f->refeicao, 'H');
            push (&f->refeicao, 'Q');
            push (&f->refeicao, 'P');
        break;

        case 1:
            push (&f->refeicao, 'p');
            push (&f->refeicao, 'H');
            push (&f->refeicao, 'S');
            push (&f->refeicao, 'P');
        break;

        case 2:
            push (&f->refeicao, 'p');
            push (&f->refeicao, 'H');
            push (&f->refeicao, 'Q');
            push (&f->refeicao, 'P');
            push (&f->refeicao, 'F');
            push (&f->refeicao, 'R');
        break;

        case 3:
            push (&f->refeicao, 'p');
            push (&f->refeicao, 'H');
            push (&f->refeicao, 'S');
            push (&f->refeicao, 'P');
            push (&f->refeicao, 'F');
            push (&f->refeicao, 'R');
        break;

        case 4:
            push (&f->refeicao, 'p');
            push (&f->refeicao, 'Q');
            push (&f->refeicao, 'P');
            push (&f->refeicao, 'F');
            push (&f->refeicao, 'R');
        break;

        case 5:
            push (&f->refeicao, 'S');
            push (&f->refeicao, 'F');
            push (&f->refeicao, 'R');
        break;

        default:
        break;
    }
    f->prox = NULL;
    f->ant = NULL;
    return f;
}

/*Remove o início da fila*/
struct nof *dequeue (struct fila **fila){
    struct nof *aux;
    aux = (*fila)->inicio;
    if ((*fila)->inicio->prox)
        (*fila)->inicio->prox->ant = NULL;
    (*fila)->inicio = (*fila)->inicio->prox;
    (*fila)->tam--;
    return aux;
}

/*Adiciona um nó no final da fila*/
int enqueue (struct fila **fila, int cliente, int refeicao){
    struct nof *aux;
    aux = criaNof(cliente, refeicao);
    if (!(*fila)->tam){
        (*fila)->fim = aux;
        (*fila)->inicio = aux;
    }
    else{
        aux->ant = (*fila)->fim;
        (*fila)->fim->prox = aux;
        (*fila)->fim = aux;
    }
    (*fila)->tam++;
    return 0;
}

/*Realiza a alocação da fila e inicializa suas variáveis. Caso haja falha na alocação, o programa sai com um código de erro*/
int inicializaFila (struct fila **fila){
    (*fila) = malloc (sizeof(struct fila));
    if (*fila){
        (*fila)->tam = 0;
        (*fila)->inicio = NULL;
        (*fila)->fim = NULL;
        return 0;
    }
    printw ("Falha ao alocar memória!\n");
    exit (1);
}

/*Mostra o primeiro elemento da fila*/
void mostraInicio (struct nof *inicio){
   printw ("Início da fila: Cliente- %d Pedido - %d\n", inicio->cliente, inicio->numRef);
}

/*Mostra o último elemento da fila*/
void mostraFim (struct nof *fim){
   printw ("Fim da fila: Cliente- %d Pedido - %d\n", fim->cliente, fim->numRef);
}

/*A função retorna 1 caso a fila esteja vazia e 0 caso não esteja*/
int estaVaziaFila (struct fila *fila){
   if (!fila->tam)
       return 1;
   return 0;
}

/*Destrói um único nó da fila, resetando todos os seus valores e dando free na pilha inserida no nó e também no nó em si*/
void destroiNof (struct nof ** nof){
    destroiPilha (&(*nof)->refeicao);
    (*nof)->cliente = 0;
    (*nof)->refeicao = 0;
    (*nof)->ant = NULL;
    (*nof)->prox = NULL;
    free (*nof);
    *nof = NULL;
}

/*Destrói a fila por completo, resetando todos os seus valores e dando free em cada nó, e por fim na fila*/
void destroiFila (struct fila **fila){
    struct nof *aux;
    while ((*fila)->inicio){
        aux = (*fila)->inicio;
        (*fila)->inicio = (*fila)->inicio->prox;
        destroiPilha (&aux->refeicao);
        aux->cliente = 0;
        aux->refeicao = 0;
        free (aux);
        aux = NULL;
    }
    free (*fila);
    *fila = NULL;
}