#ifndef LIBPILHA_H
#define LIBPILHA_H

struct nop{
    char dado;
    struct nop *prox;
};

struct pilha{
    int tam;
    int max;
    struct nop *topo;
};

struct nop *criaNop (char dado);

struct nop *pop (struct pilha **pilha);

int push (struct pilha **pilha, char dado);

void mostraTopo (struct nop *topo);

int estaVaziaPilha (struct pilha *pilha);

int inicializaPilha (struct pilha **pilha);

void imprimeRefeicao (struct nop *topo);

void destroiPilha (struct pilha **pilha);

#endif /* LIBPILHA_H */