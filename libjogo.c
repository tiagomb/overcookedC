#include "libjogo.h"
#include <ncurses.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

//Imprime um menu com arte ASCII, no qual o usuário pode selecionar o modo de jogo que deseja
void imprimeMenu (struct jogo *jogo){
    int x;
    printw ("      :::::::::       ::::::::::       :::::::::      :::    :::       :::::::::       ::::::::       ::::::::::       :::::::::\n");
    printw ("     :+:    :+:      :+:              :+:    :+:     :+:    :+:       :+:    :+:     :+:    :+:      :+:              :+:    :+:\n");
    printw ("    +:+    +:+      +:+              +:+    +:+     +:+    +:+       +:+    +:+     +:+             +:+              +:+    +:+\n");
    printw ("   +#+    +:+      +#++:++#         +#++:++#+      +#+    +:+       +#++:++#:      :#:             +#++:++#         +#++:++#:\n");
    printw ("  +#+    +#+      +#+              +#+    +#+     +#+    +#+       +#+    +#+     +#+   +#+#      +#+              +#+    +#+\n");
    printw (" #+#    #+#      #+#              #+#    #+#     #+#    #+#       #+#    #+#     #+#    #+#      #+#              #+#    #+#\n");
    printw ("#########       ##########       #########       ########        ###    ###      ########       ##########       ###    ###\n");
    printw ("\nDurante o jogo, use as teclas WASD e setas para se movimentar e Q para sair!\n");   
    printw ("\nPressione qualquer tecla para continuar\n");
    getch();
    clear();
    printw ("Selecione o modo de jogo:\n\t1-Modo normal (você escolhe a quantidade de pedidos!)\n\t2-Modo infinito (pedidos chegam até vocẽ perder ou cansar ;D)\n\t3-Modo por fases (5 fases que ficam cada vez mais difíceis!)\n");
    x = getch();
    while (x != '1' && x != '2' && x != '3')
        x = getch();
    jogo->modo = x;
    clear();
}

//Imprime uma arte ASCII e uma mensagem dizendo se o usuário ganhou ou perdeu
void imprimeFinal (int status){
    printw ("      ::::::::           :::          :::   :::       ::::::::::        ::::::::    :::     :::       ::::::::::       :::::::::\n");
    printw ("     :+:    :+:        :+: :+:       :+:+: :+:+:      :+:              :+:    :+:   :+:     :+:       :+:              :+:    :+:\n");
    printw ("    +:+              +:+   +:+     +:+ +:+:+ +:+     +:+              +:+    +:+   +:+     +:+       +:+              +:+    +:+\n");
    printw ("   :#:             +#++:++#++:    +#+  +:+  +#+     +#++:++#         +#+    +:+   +#+     +:+       +#++:++#         +#++:++#:\n");
    printw ("  +#+   +#+#      +#+     +#+    +#+       +#+     +#+              +#+    +#+    +#+   +#+        +#+              +#+    +#+\n");
    printw (" #+#    #+#      #+#     #+#    #+#       #+#     #+#              #+#    #+#     #+#+#+#         #+#              #+#    #+#\n");
    printw ("########       ###     ###    ###       ###     ##########        ########        ###           ##########       ###    ###\n");
    if (status)
        printw ("\n\t\t\t\t\t\tVocê perdeu! :(\n");
    else
        printw ("\n\t\t\t\t\t\tVocê venceu! :)\n");
    refresh();
    sleep(5);
    clear();
}

//Caso o jogador tenha vidas restantes e menos que 5 pedidos descartados (está vivo), retorna 1, caso contrário retorna 0
int jogadorVivo (struct jogo *jogo){
    return (jogo->chapeiro->vidas && jogo->chapeiro->pedidosDescartados < 5);
}

//Retorna o tempo que o usuário ainda tem para realizar o pedido
int tempoRestante (struct temporizador *timer){
    return (timer->tempo-(timer->tAtual-timer->tInicial));
}

/*Restaura o personagem às condições iniciais*/
void resetaPersonagem (struct jogo *jogo){
    jogo->chapeiro->vidas = 3;
    jogo->chapeiro->pedidosDescartados = 0;
    jogo->tela->mapa[jogo->chapeiro->lin][jogo->chapeiro->col] = ' ';
    jogo->chapeiro->lin = 3;
    jogo->chapeiro->col = 17;
    jogo->tela->mapa[jogo->chapeiro->lin][jogo->chapeiro->col] = '&';
}

//Lê o mapa de um arquivo e inicializa as strings que ficarão aparecendo na tela para auxiliar o usuário
void inicializaTela (struct tela **tela){
    FILE *matriz;
    *tela = malloc (sizeof(struct tela));
    if (!(*tela)){
        printw ("Falha ao alocar memória!\n");
        exit (1);
    }
    (*tela)->refeicoes[0] = "X-Burger";
    (*tela)->refeicoes[1] = "X-Salada";
    (*tela)->refeicoes[2] = "Combo 1";
    (*tela)->refeicoes[3] = "Combo 2";
    (*tela)->refeicoes[4] = "Vegetariano";
    (*tela)->refeicoes[5] = "Vegano";
    (*tela)->receitas[0] = "[p][H][Q][P]";
    (*tela)->receitas[1] = "[p][H][S][P]";
    (*tela)->receitas[2] = "[p][H][Q][P][F][R]";
    (*tela)->receitas[3] = "[p][H][S][P][F][R]";
    (*tela)->receitas[4] = "[p][Q][P][F][R]";
    (*tela)->receitas[5] = "[S][F][R]";
    (*tela)->mensagem = "\0";
    matriz = fopen ("matriz", "r");
    if (!matriz){
        printw ("Arquivo de mapa inexistente!\n");
        exit (1);
    }
    for (int i = 0; i<LIN; i++){
        for (int j=0; j<COL; j++)
            fscanf (matriz, "%c", &(*tela)->mapa[i][j]);
        fgetc(matriz);
    }
    fclose (matriz);
}

//Inicializa todas as variáveis relacionadas ao personagem (as posições são predeterminadas pelo mapa utilizado)
void inicializaPersonagem (struct personagem **chapeiro){
    *chapeiro = malloc (sizeof (struct personagem));
    if (!(*chapeiro)){
        printw ("Falha ao alocar memória!\n");
        exit (1);
    }
    (*chapeiro)->lin = 3;
    (*chapeiro)->col = 17;
    (*chapeiro)->vidas = 3;
    (*chapeiro)->pedidosDescartados = 0;
    (*chapeiro)->pedidosJogados = 0;
}

//Inicializa o timer e suas variáveis em 0
void inicializaTimer (struct temporizador **timer){
    *timer = malloc (sizeof (struct temporizador));
    if (!(*timer)){
        printw ("Falha ao alocar memória!\n");
        exit (1);
    }
    (*timer)->tempo = 0;
    (*timer)->tInicial = 0;
    (*timer)->tAtual = 0;
}

//Inicializa o jogo por completo, através da chamada de outras funções de inicialização
void inicializaJogo(struct jogo **jogo){
    *jogo = malloc (sizeof(struct jogo));
    if (!(*jogo)){
        printw ("Falha ao alocar memória!\n");
        exit (1);
    }
    inicializaTimer (&(*jogo)->timer);
    inicializaFila (&(*jogo)->clientes);
    inicializaPilha (&(*jogo)->refeicao);
    inicializaTela (&(*jogo)->tela);
    inicializaPersonagem (&(*jogo)->chapeiro);
}

//Configura o timer com um tempo passado por outra função, inicializando os tempos inicial e final com o mesmo valor
void setaTimer (struct temporizador *timer, int tempo){
    timer->tempo = tempo;
    timer->tInicial = time(NULL);
    timer->tAtual = timer->tInicial;
}

//Essa função é chamada dentro de um loop, e imprime todas as partes necessárias para que o jogo apareça como esperado para o usuário (necessário tela cheia)
void imprimeJogo(struct jogo *jogo){
    struct nof *aux = jogo->clientes->inicio;
    int x = 0;
    printw ("#--------------------#--------------------#---------------------------------#\n");
    printw ("| Tempo restante: %d | Vidas restantes: %d | Refeições descartadas: %d Max: 5 |\n", tempoRestante (jogo->timer), jogo->chapeiro->vidas, jogo->chapeiro->pedidosDescartados);
    printw ("#--------------------#--------------------#---------------------------------#\n");
    for (int i = 0; i<LIN; i++){
        for (int j = 0; j<COL; j++)
            printw ("%c", jogo->tela->mapa[i][j]);
        if (!i){
            printw (" ");
            while (aux && x<5){
                //usa o número da refeição para acessar o índice correspondente no vetor de strings que contém o nome das refeições e imprimí-la
                printw ("[Cliente %d | %s] ", aux->cliente, jogo->tela->refeicoes[aux->numRef]);
                aux = aux->prox;
                if (aux && x != 4)
                    printw ("-> ");
                x++;
            }
        }
        else if (i==2){
            aux = jogo->clientes->inicio;
            if (aux)
                //usa o número da refeição para acessar os índices correspondentes no vetor de strings que contém o nome das refeições e no que contém as receitas
                printw (" Receita %s: %s", jogo->tela->refeicoes[aux->numRef], jogo->tela->receitas[aux->numRef]);
        }
        printw ("\n");
    }
    printw ("Ingredientes adicionados: ");
    imprimeRefeicao (jogo->refeicao->topo);
    printw ("\nAvisos: %s", jogo->tela->mensagem);
}

/*Recebe as coordenadas a serem testadas de outra função e verifica se o caractere dessas cordenadas corresponde a alguma instrução a ser executada. 
Caso corresponda, executa essa instrução*/
void testaColisao (struct jogo *jogo, int lin, int col){
    switch (jogo->tela->mapa[lin][col]){
        case 'R':
            push (&jogo->refeicao, 'R');
        break;
        case 'F':
            push (&jogo->refeicao, 'F');
        break;
        case 'p':
            push (&jogo->refeicao, 'p');
        break;
        case 'P':
            push (&jogo->refeicao, 'P'); 
        break;
        case 'Q':
            push (&jogo->refeicao, 'Q'); 
        break;
        case 'H':
            push (&jogo->refeicao, 'H');
        break;
        case 'S':
            push (&jogo->refeicao, 'S');
        break;
        case '@':
            verificaPedido (jogo);
        break;
        case 'o':
            if (destroiRefeicao (&jogo->refeicao))
                jogo->tela->mensagem = "Não há refeição para ser descartada!";
            else{
                jogo->tela->mensagem = "Refeição descartada!";
                jogo->chapeiro->pedidosDescartados++;
            }
        break;
    }
}

/*A função interpreta o caractere pressionado pelo usuário e realiza a movimentação em conformidade. Caso o espaço que o usuário deseja ocupar esteja livre,
realiza a troca de posições, caso contrário, chama a função que testa colisão com as coordenadas que estão no sentido no qual o usuário tentou avançar.*/
void andaPersonagem (struct jogo *jogo){
    int direcao = 0;
    timeout(1000); //Espera uma ação do usuário por 1 segundo e então sai da função, permitindo que o timer seja atualizado a cada segundo
    direcao = getch();
    jogo->tela->mensagem = "\0"; //reseta a mensagem para que ela não fique aparecendo durante muito tempo na tela
    switch (direcao){
        case 'W':
        case 'w':
        case KEY_UP:
            if (jogo->tela->mapa[jogo->chapeiro->lin - 1][jogo->chapeiro->col] == ' '){
                jogo->tela->mapa[jogo->chapeiro->lin][jogo->chapeiro->col] = ' ';
                jogo->tela->mapa[--jogo->chapeiro->lin][jogo->chapeiro->col] = '&';
            }
            else
                testaColisao (jogo, jogo->chapeiro->lin - 1, jogo->chapeiro->col);
        break;

        case 'A':
        case 'a':
        case KEY_LEFT:
            if (jogo->tela->mapa[jogo->chapeiro->lin][jogo->chapeiro->col - 1] == ' '){
                jogo->tela->mapa[jogo->chapeiro->lin][jogo->chapeiro->col] = ' ';
                jogo->tela->mapa[jogo->chapeiro->lin][--jogo->chapeiro->col] = '&';
            }
            else
                testaColisao (jogo, jogo->chapeiro->lin, jogo->chapeiro->col - 1);
        break;

        case 'S':
        case 's':
        case KEY_DOWN:
            if (jogo->tela->mapa[jogo->chapeiro->lin + 1][jogo->chapeiro->col] == ' '){
                jogo->tela->mapa[jogo->chapeiro->lin][jogo->chapeiro->col] = ' ';
                jogo->tela->mapa[++jogo->chapeiro->lin][jogo->chapeiro->col] = '&';
            }
            else
                testaColisao (jogo, jogo->chapeiro->lin + 1, jogo->chapeiro->col);
        break;

        case 'D':
        case 'd':
        case KEY_RIGHT:
            if (jogo->tela->mapa[jogo->chapeiro->lin][jogo->chapeiro->col + 1] == ' '){
                jogo->tela->mapa[jogo->chapeiro->lin][jogo->chapeiro->col] = ' ';
                jogo->tela->mapa[jogo->chapeiro->lin][++jogo->chapeiro->col] = '&';
            }
            else
                testaColisao (jogo, jogo->chapeiro->lin, jogo->chapeiro->col + 1);
        break;
            
        case 'Q':
        case 'q':
            printw("Saindo...\n");
            destroiJogo (&jogo);
            refresh();
            sleep(3);
            clear();
            endwin();
            exit(0);
        break;
    }
    return;
}

//A função compara a refeição preparada pelo usuário com a que chegou através do cliente. Caso sejam iguais, retorna 0. Em caso contrário, retorna 1
int comparaRefeicao (struct jogo *jogo){
    struct nop *aux, *aux1;
    if (jogo->refeicao->tam != jogo->clientes->inicio->refeicao->tam){
        destroiRefeicao (&jogo->refeicao);
        return 1;
    }
    aux = jogo->refeicao->topo;
    aux1 = jogo->clientes->inicio->refeicao->topo;
    while (aux){
        if (aux->dado != aux1->dado){
            destroiRefeicao (&jogo->refeicao);
            return 1;
        }
        aux = aux->prox;
        aux1 = aux1->prox;
    }
    destroiRefeicao (&jogo->refeicao);
    return 0;
}

//A função é chamada quando o usuário encosta no '@', e verifica se o pedido está conforme solicitado, colocando em uma string se o resultado foi positivo ou não
void verificaPedido (struct jogo *jogo){
    struct nof *cliente;
    int vidasAntes = jogo->chapeiro->vidas;
    if (comparaRefeicao (jogo))
        jogo->chapeiro->vidas--;
    jogo->chapeiro->pedidosJogados++;
    cliente = dequeue (&jogo->clientes);
    destroiNof (&cliente);
    if (jogo->modo == '2') //caso o modo seja infinito, toda vez que um pedido é entregue outro é adicionado, evitando assim alocação excessiva de memória
        enqueue (&jogo->clientes, jogo->chapeiro->pedidosJogados + 1, rand()%5);
    jogo->timer->tInicial = time(NULL); //reseta o timer
    if (vidasAntes > jogo->chapeiro->vidas)
        jogo->tela->mensagem = "Cliente foi embora insatisfeito, você perdeu uma vida!\n";
    else
        jogo->tela->mensagem = "Cliente foi embora satisfeito, continue assim!\n";
}

//A função é chamada caso a função tempoRestante retorne 0. Retira uma vida do usuário e o cliente da fila. Além disso, coloca em uma string uma mensagem de aviso
void acabouTempo (struct jogo *jogo){
    struct nof *cliente;
    cliente = dequeue (&jogo->clientes);
    destroiNof (&cliente);
    destroiRefeicao (&jogo->refeicao);
    jogo->chapeiro->vidas--;
    jogo->tela->mensagem = "Cliente foi embora insatisfeito, você perdeu uma vida!\n";
    jogo->timer->tInicial = time(NULL); //reseta o timer
}

/*Essa função faz o jogo se comportar conforme o enunciado. Pede ao usuário o número de pedidos que ele deseja jogar e gera-os aleatoriamente.
Caso o usuário realize os pedidos sem perder suas vidas ou descartar muitos pedidos, ele vence.*/
void modoNormal (struct jogo *jogo){
    int pedidos;
    echo();
    printw ("Insira a quantidade de pedidos que deseja jogar e pressione ENTER: ");
    scanw ("%d", &pedidos);
    noecho();
    for (int i = 0; i<pedidos;i++)
        enqueue (&jogo->clientes, i+1, rand()%5);
    setaTimer (jogo->timer, 30);
    while (jogadorVivo (jogo) && !estaVaziaFila(jogo->clientes)){
        imprimeJogo (jogo);
        andaPersonagem (jogo);
        refresh();
        clear();
        jogo->timer->tAtual = time(NULL); //atualiza o timer
        if (!tempoRestante(jogo->timer)){
            acabouTempo (jogo);
        }
    }
    if (!jogadorVivo (jogo))
        imprimeFinal (1);
    else
        imprimeFinal (0);
}

/*Nessa função, os pedidos vêm conforme o usuário os entrega (são adicionados na função verificaPedido), parando somente quando o usuário cansa e sai, ou quando
suas vidas acabam ou ele descarta 5 pedidos*/
void modoInfinito (struct jogo *jogo){
    enqueue (&jogo->clientes, 1, rand()%5);
    setaTimer (jogo->timer, 30);
    while (jogadorVivo (jogo) && !estaVaziaFila(jogo->clientes)){
        imprimeJogo (jogo);
        andaPersonagem (jogo);
        refresh();
        clear();
        jogo->timer->tAtual = time(NULL); //atualiza o timer
        if (!tempoRestante(jogo->timer)){
            acabouTempo (jogo);
        }
    }
    if (!jogadorVivo (jogo))
        imprimeFinal (1);
    else
        imprimeFinal (0);
}

/*Nessa função, o jogo funciona com 5 fases. Na primeira fase há 3 pedidos com 40 segundos pro usuário resolver, e a cada fase o número de pedidos aumenta em 3
enquanto o tempo diminui em 5. As vidas e pedidos descartados resetam a cada fase*/
void modoFase (struct jogo *jogo){
    int tempo, fases;
    tempo = 45;
    for (fases = 1; fases<=5; fases++){
        printw ("\n\n\n\t\tFase %d", fases);
        refresh();
        sleep(3);
        clear();
        for (int i = 0; i<fases*3; i++)
            enqueue (&jogo->clientes, i+1, rand()%5);
        setaTimer (jogo->timer, tempo -= 5); //diminui o tempo a cada loop
        while (jogadorVivo (jogo) && !estaVaziaFila(jogo->clientes)){
            imprimeJogo (jogo);
            andaPersonagem (jogo);
            refresh();
            clear();
            jogo->timer->tAtual = time(NULL); //atualiza o timer
            if (!tempoRestante(jogo->timer)){
                acabouTempo (jogo);
            }
        }
        if (!jogadorVivo (jogo)){
            imprimeFinal (1);
            return;
        }
        resetaPersonagem (jogo);
    }
    if (!jogadorVivo (jogo))
        imprimeFinal (1);
    else
        imprimeFinal (0);
}

//Quando o usuário encosta na lixeira ou entrega um pedido, essa função é chamada para remover todos os ingredientes da pilha, deixando-a vazia
int destroiRefeicao (struct pilha **refeicao){
    struct nop *removido;
    if (!estaVaziaPilha (*refeicao)){
        while ((*refeicao)->tam){
            removido = pop (refeicao);
            removido->dado = '0';
            free (removido);
        }
        return 0;
    }
    return 1;
}

//Faz todas as strings receberem o caractere nulo, e então da free no ponteiro para a struct e faz seu valor ser nulo
void destroiTela (struct tela **tela){
    if (*tela){
        (*tela)->receitas[0] = "\0";
        (*tela)->refeicoes[0] = "\0";
        (*tela)->mensagem = "\0";
        (*tela)->mapa[0][0] = '\0';
        free (*tela);
        *tela = NULL;
    }
}

//Faz todos os valores receberem 0, e então da free no ponteiro para a struct e faz seu valor ser nulo
void destroiPersonagem (struct personagem **chapeiro){
    if (*chapeiro){
        (*chapeiro)->pedidosDescartados = 0;
        (*chapeiro)->pedidosJogados = 0;
        (*chapeiro)->lin = 0;
        (*chapeiro)->col = 0;
        (*chapeiro)->vidas = 0;
        free (*chapeiro);
        *chapeiro = NULL;
    }
}

//Faz todos os valores receberem 0, e então da free no ponteiro para a struct e faz seu valor ser nulo
void destroiTimer (struct temporizador **timer){
    if (*timer){
        (*timer)->tempo = 0;
        (*timer)->tAtual = 0;
        (*timer)->tInicial = 0;
        free (*timer);
        *timer = NULL;
    }
}

//Destrói o jogo por completo através das chamadas de outras funções de destruição. Por fim, da free no ponteiro para a struct e faz seu valor ser nulo
void destroiJogo (struct jogo **jogo){
    if (*jogo){
        destroiFila (&(*jogo)->clientes);
        destroiPilha(&(*jogo)->refeicao);
        destroiTela (&(*jogo)->tela);
        destroiPersonagem (&(*jogo)->chapeiro);
        destroiTimer (&(*jogo)->timer);
        free (*jogo);
        *jogo = NULL;
    }
}