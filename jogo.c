#include "libjogo.h"
#include <ncurses.h>
#include <time.h>
#include <stdlib.h>

int main(){
    struct jogo *jogo;
    srand(time(NULL)); //inicia a seed para inserir pedidos aleatórios
    initscr();  //inicia a janela do ncurses e configura com os comandos abaixo
    keypad(stdscr, TRUE);
    cbreak();
    noecho();
    inicializaJogo (&jogo);
    imprimeMenu (jogo);
    switch (jogo->modo){ //O modo foi selecionado pelo usuário dentro da função imprimeMenu
        case '1':
            modoNormal (jogo);
        break;

        case '2':
            modoInfinito (jogo);
        break;

        case '3':
            modoFase (jogo);
        break;
    }
    destroiJogo (&jogo);
    endwin(); //fecha a janela do ncurses
    return 0;
}