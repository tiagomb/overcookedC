#ifndef LIBJOGO_H
#define LIBJOGO_H

#include "libfila.h"
#include "libpilha.h"
#include <time.h>

#define COL 35
#define LIN 9

struct tela{
    char mapa[LIN][COL];
    char *refeicoes[6];
    char *receitas[6];
    char *mensagem;
};

struct personagem{
    int lin;
    int col;
    int vidas;
    int pedidosDescartados;
    int pedidosJogados;
};

struct temporizador{
    int tempo;
    time_t tInicial;
    time_t tAtual;
};

struct jogo{
    struct tela *tela;
    struct fila *clientes;
    struct pilha *refeicao;
    struct personagem *chapeiro;
    struct temporizador *timer;
    int modo;
};

void imprimeMenu (struct jogo *jogo);

void imprimeFinal (int status);

int jogadorVivo (struct jogo *jogo);

int tempoRestante (struct temporizador *timer);

void resetaPersonagem (struct jogo *jogo);

void inicializaTela (struct tela **tela);

void inicializaPersonagem (struct personagem **chapeiro);

void inicializaTimer (struct temporizador **timer);

void inicializaJogo(struct jogo **jogo);

void setaTimer (struct temporizador *timer, int tempo);

void imprimeJogo(struct jogo *jogo);

void testaColisao (struct jogo *jogo, int lin, int col);

void andaPersonagem (struct jogo *jogo);

int comparaRefeicao (struct jogo *jogo);

void verificaPedido (struct jogo *jogo);

void acabouTempo (struct jogo *jogo);

void modoNormal (struct jogo *jogo);

void modoInfinito (struct jogo *jogo);

void modoFase (struct jogo *jogo);

int destroiRefeicao (struct pilha **refeicao);

void destroiTela (struct tela **tela);

void destroiPersonagem (struct personagem **chapeiro);

void destroiTimer (struct temporizador **timer);

void destroiJogo (struct jogo **jogo);

#endif /* LIBJOGO_H */