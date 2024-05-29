#ifndef LIBFILA_H
#define LIBFILA_H

#include "libpilha.h"

struct nof{
   int cliente;
   int numRef;
   struct pilha *refeicao;
   struct nof *ant;
   struct nof *prox;
};

struct fila{
   int tam;
   struct nof *inicio;
   struct nof *fim;
};

struct nof *criaNof (int cliente, int refeicao);

struct nof *dequeue (struct fila **fila);

int enqueue (struct fila **fila, int cliente, int refeicao);

int inicializaFila (struct fila **fila);

void mostraInicio (struct nof *inicio);

void mostraFim (struct nof *fim);

int estaVaziaFila (struct fila *fila);

void destroiNof (struct nof ** nof);

void destroiFila (struct fila **fila);

#endif /* LIBFILA_H */