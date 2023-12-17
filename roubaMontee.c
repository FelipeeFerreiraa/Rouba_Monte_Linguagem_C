#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>   //pode n funcionar
#include <windows.h> //pode n funcionar

#define ANSI_COLOR_RED "\x1b[31m"    // cores em ANSI vermelho
#define ANSI_COLOR_YELLOW "\033[33m" // cores em ANSI amarelo
#define ANSI_COLOR_NONE "\033[0m"    // cores em ANSI volta a cor ao normal

#define totalCartas 54 // 52 cartas de 1 a 13, e 2 coringas por baralho

typedef struct carta
{
    int valor; // 1=ás , J=12, Q=11, K=13, joker=14
    int naipe; // 1-ouros, 2-paus, 3-espadas, 4-copas, 5-joker
               // struct carta *proxCarta;
} carta;

typedef struct no
{
    struct no *prox;
    carta cartaDaVez;
} no;

typedef struct pilhaDeCompra //'pilha' de cartas para 'comprar'
{
    no *topo;
} pilhaDeCompra;

typedef struct baralhoDesviradoMesa //'lista' de, cartas viradas na mesa
{
    no *inicio;
} baralhoDesviradoMesa;

typedef struct areaDeDescarte //'pilha' area de descarte, de cada jogador
{
    no *topo;
} areaDeDescarte;

typedef struct jogador
{
    char nome[30];
} jogador;

pilhaDeCompra *criarPilhaDeCompra();
baralhoDesviradoMesa *criarBaralhoDesviradoMesa();
areaDeDescarte **criarAreaDeDescarte(int qtdeJogadores);
// retornar um vetor de pilhas de descarte de cada jogador;

// inserindo todas pelo inicio
int inserirPilhaDeCompra(pilhaDeCompra *p, carta c);
int inserirBaralhoDesviradoMesa(baralhoDesviradoMesa *b, carta c);
int inserirAreaDeDescarte(areaDeDescarte *a, carta c);

// removendo todas pelo inicio
int removerPilhaDeCompra(pilhaDeCompra *p, carta *res);
int removerAreaDeDescarte(areaDeDescarte *a, carta *res);
int removerBaralhoDesviradoMesa(baralhoDesviradoMesa *b, carta *res);
int removerBaralhoDesviradoMesaPOS(baralhoDesviradoMesa *b, carta *res, int pos);

void criandoBaralho(carta *baralho, int tam, int qtdeBaralhos); // vetor de cartas
void embaralhar(carta *baralho, int tam);
void imprimirBaralho(carta *baralho, int tam);

void colocandoBaralhoPilhaCompra(carta *baralho, pilhaDeCompra *p1, int tam);
void qtdeCartasPilhaCompra(pilhaDeCompra *p);

void NomearJogadores(jogador *j1, int qtde);

int verificarCartasBaralhoMesa(carta *p, baralhoDesviradoMesa *b);

int verificarCartasDescarteJogadores(carta *retirada, areaDeDescarte **a, int qtdeJogadores);

void imprimirBaralhoMesa(baralhoDesviradoMesa *b);
void imprimirTopoDescarte(areaDeDescarte **a, int qtdeJogadores, jogador *j1);

void colocarCartaAreaDescarteJogador(areaDeDescarte **a, int jogador, carta c);
void roubarMonte(areaDeDescarte **a, int jogadorDaVez, int jogadorAlvo);
int qtdeCartasMonteDescarte(areaDeDescarte *a);

int verificarSeEhCoringa(carta retirada);
int vencedorMaisCartas(areaDeDescarte **a, int qtdeJogadores, jogador *j1, baralhoDesviradoMesa **cartasJogadores, int *maisCartas, int *vencedor, int *empate);

int verificarAreaDescarte(areaDeDescarte **a, int qtdeJogadores, carta retirada);
int inserirBaralhoMesaOrdenado(baralhoDesviradoMesa *a, carta c);

void imprimirAreasDescarte(areaDeDescarte **a, int qtdeJogadores, jogador *j1);

baralhoDesviradoMesa **criarMaoFinalJogadores(int qtdeJogadores);

void converterCarta(baralhoDesviradoMesa valor);
void converterNaipe(baralhoDesviradoMesa naipe);

int main()
{
    srand(time(NULL));

    int qtdeBaralhos, qtdeJogadores;

    printf("\nJogo Rouba-Monte!!!\n");

    //-----------quantidade de jogadores
    printf("\nQuantos Jogadores vao Jogaaar!\n");
    setbuf(stdin, NULL);
    scanf("%d", &qtdeJogadores);
    jogador *jogadores = (jogador *)malloc(qtdeJogadores * sizeof(jogador));
    NomearJogadores(jogadores, qtdeJogadores);

    //-----------quantidade de baralhos
    printf("\nCom quantos baaralhos vamos jogar? \n");
    setbuf(stdin, NULL);
    scanf("%d", &qtdeBaralhos);
    int novoTotal = qtdeBaralhos * 54; // no minimo 1 baralho
    carta *baralhoooo = (carta *)malloc(novoTotal * sizeof(carta));

    //-----------criando e embaralhando
    criandoBaralho(baralhoooo, novoTotal, qtdeBaralhos);
    embaralhar(baralhoooo, novoTotal);
    // teste: imprimirBaralho(baralhoooo, novoTotal);

    //----------colocando as cartas na pilha de compraaa
    pilhaDeCompra *monteDeCompra = criarPilhaDeCompra();
    colocandoBaralhoPilhaCompra(baralhoooo, monteDeCompra, novoTotal);
    // teste: qtdeCartasPilhaCompra(monteDeCompra);

    //-----------'LISTA' baralho na mesa
    baralhoDesviradoMesa *baralhoNoCentro = criarBaralhoDesviradoMesa();

    //-----------vetor de 'PILHAS' de descarte para cada jogador
    areaDeDescarte **descarteDeMontes = criarAreaDeDescarte(qtdeJogadores);

    carta cartaRetirada;
    // no *auxMonteDeCompra = monteDeCompra->topo;
    int jogadorDaVez = 0, teste, teste2, testeCoringa, teste3;
    int jogadorAlvo;
    int escolha;

    while (monteDeCompra->topo != NULL)
    {
        system("cls");

        printf(ANSI_COLOR_RED "\nVez do jogador: %s \n " ANSI_COLOR_NONE, jogadores[jogadorDaVez].nome);

        if (removerPilhaDeCompra(monteDeCompra, &cartaRetirada))
        {
            printf(ANSI_COLOR_YELLOW "\nCarta retirada: valor: %d, naipe: %d\n " ANSI_COLOR_NONE, cartaRetirada.valor, cartaRetirada.naipe);
        }
        else
        {
            printf("\nERRO\n");
        }

        imprimirBaralhoMesa(baralhoNoCentro);
        imprimirTopoDescarte(descarteDeMontes, qtdeJogadores, jogadores);

        do
        {

            printf("\nEscolha:\n[1] Pegar carta do baralho na mesa\n[2] Roubar o monte de algum jogador\n[3] Colocar carta no meu monte\n[4] Colocar carta no baralho da mesa, virada pra cima!\n");
            setbuf(stdin, NULL);
            scanf("%d", &escolha);

            testeCoringa = 0;
            teste2 = 0;
            teste = 0;
            teste3 = -1;

            switch (escolha)
            {
            case 1:
                testeCoringa = verificarSeEhCoringa(cartaRetirada); // 1 curinga, 0 nao eh

                if (cartaRetirada.valor == baralhoNoCentro->inicio->cartaDaVez.valor)
                {
                    int testeeeee = 0;
                    carta aux;
                    removerBaralhoDesviradoMesaPOS(baralhoNoCentro, &aux, testeeeee);

                    inserirAreaDeDescarte(descarteDeMontes[jogadorDaVez], aux); // carta q tava na mesa

                    inserirAreaDeDescarte(descarteDeMontes[jogadorDaVez], cartaRetirada); // carta retirada do monte de compras
                    break;
                }

                if (testeCoringa == 0 && teste2 == 0)
                {
                    teste = verificarCartasBaralhoMesa(&cartaRetirada, baralhoNoCentro);

                    if (teste != 0)
                    {
                        carta aux;
                        removerBaralhoDesviradoMesaPOS(baralhoNoCentro, &aux, teste);

                        inserirAreaDeDescarte(descarteDeMontes[jogadorDaVez], aux); // carta q tava na mesa

                        inserirAreaDeDescarte(descarteDeMontes[jogadorDaVez], cartaRetirada); // carta retirada do monte de compras
                    }
                }
                break;

            case 2:

                jogadorAlvo = verificarCartasDescarteJogadores(&cartaRetirada, descarteDeMontes, qtdeJogadores);

                if (jogadorAlvo >= 0 && jogadorAlvo < qtdeJogadores && descarteDeMontes[jogadorAlvo] != NULL)
                {
                    roubarMonte(descarteDeMontes, jogadorDaVez, jogadorAlvo);

                    inserirAreaDeDescarte(descarteDeMontes[jogadorDaVez], cartaRetirada);

                    break;
                }

            case 3:

                colocarCartaAreaDescarteJogador(descarteDeMontes, jogadorDaVez, cartaRetirada);

                break;

            case 4:
                testeCoringa = verificarSeEhCoringa(cartaRetirada);

                teste2 = verificarCartasBaralhoMesa(&cartaRetirada, baralhoNoCentro);

                teste3 = verificarAreaDescarte(descarteDeMontes, qtdeJogadores, cartaRetirada);

                inserirBaralhoDesviradoMesa(baralhoNoCentro, cartaRetirada);

                // inserirBaralhoMesaOrdenado(baralhoNoCentro, cartaRetirada);

                jogadorDaVez++;
                if (jogadorDaVez >= qtdeJogadores)
                {
                    jogadorDaVez = 0;
                }
                break;

            default:
                testeCoringa = 1; // repetir a escolha novamente caso digite um numero errado
                break;
            }

        } while (teste2 > 0 || testeCoringa >= 1 || teste3 >= 0); // teste == -1 || posNoVetorPonteiros <= -1  ||
    }

    printf("\nAcabou as cartas!!!1\n");

    baralhoDesviradoMesa **cartasJogadores = criarMaoFinalJogadores(qtdeJogadores);
    int vencedor, maisCartas, empate;

    if (vencedorMaisCartas(descarteDeMontes, qtdeJogadores, jogadores, cartasJogadores, &maisCartas, &vencedor, &empate))
    {
        if (empate == 0)
        {
            printf("\n==========+++++++=####&&&&&&$$$$$ GANHADOR: Com %d cartas, %s \n", maisCartas, jogadores[vencedor].nome);
        }
        else
        {
            printf("\n_________HOUVE EMPATE________\n");
            printf("\n==========+++++++=####&&&&&&$$$$$ GANHADOR: Com %d cartas, %s \n", maisCartas, jogadores[vencedor].nome);
            printf("\n==========+++++++=####&&&&&&$$$$$ GANHADOR: Com %d cartas, %s \n", maisCartas, jogadores[empate].nome);
        }
    }

    free(baralhoooo);
    return 0;
}

pilhaDeCompra *criarPilhaDeCompra()
{
    pilhaDeCompra *p1 = (pilhaDeCompra *)malloc(sizeof(pilhaDeCompra));
    if (p1 != NULL)
    {
        p1->topo = NULL;
    }
    return p1;
}

baralhoDesviradoMesa *criarBaralhoDesviradoMesa()
{
    baralhoDesviradoMesa *b1 = (baralhoDesviradoMesa *)malloc(sizeof(baralhoDesviradoMesa));
    if (b1 != NULL)
    {
        b1->inicio = NULL;
    }
    return b1;
}

areaDeDescarte **criarAreaDeDescarte(int qtdeJogadores)
{
    areaDeDescarte **a1 = (areaDeDescarte **)malloc(qtdeJogadores * sizeof(areaDeDescarte *));
    for (int x = 0; x < qtdeJogadores; x++)
    {
        a1[x] = (areaDeDescarte *)malloc(sizeof(areaDeDescarte));
        a1[x]->topo = NULL;
    }
    return a1;
}

int inserirPilhaDeCompra(pilhaDeCompra *p, carta c)
{
    if (p != NULL)
    {
        no *novo = (no *)malloc(sizeof(no));
        if (novo == NULL)
        {
            return 0;
        }

        novo->cartaDaVez = c;
        if (p->topo == NULL)
        {
            novo->prox = NULL;
            p->topo = novo;
            return 1;
        }
        else
        {
            no *aux;
            aux = novo;
            aux->prox = p->topo;
            p->topo = aux;
            return 1;
        }
    }
    else
        return 0;
}

int inserirBaralhoDesviradoMesa(baralhoDesviradoMesa *b, carta c)
{
    if (b != NULL)
    {
        no *novo = (no *)malloc(sizeof(no));
        if (novo == NULL)
        {
            return 0;
        }
        novo->cartaDaVez = c;
        if (b->inicio == NULL)
        {
            novo->prox = NULL;
            b->inicio = novo;
            return 1;
        }
        else
        {
            novo->prox = b->inicio;
            b->inicio = novo;
            return 1;
        }
    }
    else
        return 0;
}

int inserirAreaDeDescarte(areaDeDescarte *a, carta c)
{

    if (a != NULL)
    {
        no *novo = (no *)malloc(sizeof(no));
        if (novo == NULL)
        {
            return 0;
        }

        novo->cartaDaVez = c;
        if (a->topo == NULL)
        {
            novo->prox = NULL;
            a->topo = novo;
            return 1;
        }
        else
        {
            no *aux;
            aux = novo;
            aux->prox = a->topo;
            a->topo = aux;
            return 1;
        }
    }
    else
    {
        printf("ERRO NA FUNÇÃO: inserirAreaDeDescarte()\n ");
        return 0;
    }
}

int removerBaralhoDesviradoMesa(baralhoDesviradoMesa *b, carta *res)
{
    if (b != NULL)
    {
        no *aux = b->inicio;
        *res = aux->cartaDaVez;
        b->inicio = aux->prox;
        free(aux);
        return 1;
    }
    else if (b->inicio == NULL)
    {
        return 0;
    }
    else
        return 0;
}

int removerBaralhoDesviradoMesaPOS(baralhoDesviradoMesa *b, carta *res, int pos)
{
    if (b != NULL)
    {
        if (b->inicio == NULL)
        {
            return 0;
        }

        if (pos == 0)
        {
            *res = b->inicio->cartaDaVez;
            b->inicio = b->inicio->prox;
            return 1;
        }

        no *ant, *atual = b->inicio;
        int cont = 1;

        while (cont != pos && atual != NULL)
        {
            ant = atual;
            atual = atual->prox;
            cont++;
        }
        ant->prox = atual->prox;
        *res = atual->cartaDaVez;
        free(atual);
        return 1;
    }
    else
        return 0;
}

int removerPilhaDeCompra(pilhaDeCompra *p, carta *res)
{
    if (p != NULL)
    {
        no *aux = p->topo;
        *res = aux->cartaDaVez;
        p->topo = aux->prox;
        free(aux);
        return 1;
    }
    else
        return 0;
}

int removerAreaDeDescarte(areaDeDescarte *a, carta *res)
{
    if (a != NULL && a->topo != NULL)
    {
        no *aux = a->topo;
        *res = aux->cartaDaVez;
        a->topo = aux->prox;
        free(aux);
        return 1;
    }
    else
    {
        printf("ERRO NA FUNÇÃO: removerAreaDeDescarte()\n ");
        return 0;
    }
}

void criandoBaralho(carta *baralho, int tam, int qtdeBaralhos) // vetor de cartas
{

    int valor[14] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
    int naipe[4] = {1, 2, 3, 4};

    int i = 0;

    // Adicionando as cartas
    while (i < tam - (2 * qtdeBaralhos))
    {
        for (int x = 0; x < 4; x++) // x < Naipes
        {
            for (int y = 0; y < 13; y++)
            {

                baralho[i].valor = valor[y];
                baralho[i].naipe = naipe[x];
                i++;
            }
        }
    }

    for (int w = i; w < tam; w++)
    {
        baralho[w].valor = valor[13];
        baralho[w].naipe = 5; // coringa tem naipe==5
    }
}

void embaralhar(carta *baralho, int tam)
{
    int x, y;  //algoritmo fishe-yates
    carta aux;

    for (x = tam - 1; x > 0; x--) 
    {
        y = rand() % (x + 1); 

        aux = baralho[x];
        baralho[x] = baralho[y];
        baralho[y] = aux;
    }
}

void imprimirBaralho(carta *baralho, int tam)
{
    int i;

    for (i = 0; i < tam; i++)
    {
        printf("%d de %d,, [%d]\n", baralho[i].valor, baralho[i].naipe, i + 1);
    }
}

void colocandoBaralhoPilhaCompra(carta *baralho, pilhaDeCompra *p1, int tam)
{

    for (int x = 0; x < tam; x++)
    {
        if (inserirPilhaDeCompra(p1, baralho[x]))
        {
        }
        else
            printf("ALGO ESTA DANDO ERRADO!!!");
    }
}

void qtdeCartasPilhaCompra(pilhaDeCompra *p)
{
    if (p != NULL)
    {
        no *aux = p->topo;
        int qtd = 0;
        while (aux != NULL)
        {
            qtd++;
            aux = aux->prox;
        }
        printf("\nqtd: %d \n", qtd);
    }
    else
        printf("ALGO ESTA DANDO ERRADO!!!");
}

void NomearJogadores(jogador *j1, int qtde)
{
    printf("\nForneca o nome dos jogadores: \n");
    for (int x = 0; x < qtde; x++)
    {
        printf("Jogador %d: ", x + 1);
        setbuf(stdin, NULL);
        fgets(j1[x].nome, 30, stdin);
    }
}

int verificarCartasBaralhoMesa(carta *p, baralhoDesviradoMesa *b)
{
    if (p == NULL || b == NULL || b->inicio == NULL)
    {
        printf("ERRO NA FUNÇÃO: verificarCartasBaralhoMesa()\n ");
        return 0;
    }

    no *auxBaralhoMesa = b->inicio;

    int posDaCartaNaMesa = 1;

    while (auxBaralhoMesa != NULL)
    {
        if (auxBaralhoMesa->cartaDaVez.valor == p->valor)
        {

            return posDaCartaNaMesa;
            // remover carta por posicao
        }

        auxBaralhoMesa = auxBaralhoMesa->prox;
        posDaCartaNaMesa++;
    }

    return 0;
}

int verificarCartasDescarteJogadores(carta *retirada, areaDeDescarte **a, int qtdeJogadores)
{

    int posVetorPonteiros;
    if (a != NULL)
    {
        for (int x = 0; x < qtdeJogadores; x++)
        {
            if (a[x]->topo != NULL && a[x]->topo->cartaDaVez.valor == retirada->valor)
            {
                posVetorPonteiros = x; // posição no vetor
                return posVetorPonteiros;
            }
        }
    }
    else
    {
        posVetorPonteiros = -1;
    }
    return posVetorPonteiros;
}

void imprimirBaralhoMesa(baralhoDesviradoMesa *b)
{
    no *aux;
    aux = b->inicio;

    printf("\n========== CARTAS NA MESA  ================\n\n");
    if (aux != NULL)
    {
        while (aux != NULL)
        {
            printf("[Valor %d, Naipe: %d ]  ", aux->cartaDaVez.valor, aux->cartaDaVez.naipe);
            aux = aux->prox;
        }
    }
    printf("\n\n========== CARTAS NA MESA ================\n");
}

void imprimirTopoDescarte(areaDeDescarte **a, int qtdeJogadores, jogador *j1)
{

    printf("\n########### TOPO DOS JOGADORES  #############\n\n");
    for (int x = 0; x < qtdeJogadores; x++)
    {
        if (a[x]->topo != NULL)
        {
            printf("\n[Topo do %sValor %d, Naipe %d] - qtdTT= %d\n  ", j1[x].nome, a[x]->topo->cartaDaVez.valor, a[x]->topo->cartaDaVez.naipe, qtdeCartasMonteDescarte(a[x]));
        }
    }

    printf("\n########### TOPO DOS JOGADORES  #############\n\n");
}

void colocarCartaAreaDescarteJogador(areaDeDescarte **a, int jogador, carta c)
{
    inserirAreaDeDescarte(a[jogador], c);
}

void roubarMonte(areaDeDescarte **a, int jogadorDaVez, int jogadorAlvo)
{
    printf("dentro rouba monte\n"); // teste
    carta auxC;

    while (a[jogadorAlvo]->topo != NULL) 
    {
        printf("aux!=NULL\n"); // teste
        if (removerAreaDeDescarte(a[jogadorAlvo], &auxC))
        {
            if (inserirAreaDeDescarte(a[jogadorDaVez], auxC))
            {
                printf("Inserido!!!\n"); // teste
            }
        }
    }
}

int qtdeCartasMonteDescarte(areaDeDescarte *a)
{
    int qtd = 0;
    no *aux = a->topo;

    while (aux != NULL)
    {
        qtd++;
        aux = aux->prox;
    }
    return qtd;
}

int qtdeCartasBaralhoMesa(baralhoDesviradoMesa *a)
{
    int qtd = 0;
    no *aux = a->inicio;

    while (aux != NULL)
    {
        qtd++;
        aux = aux->prox;
    }
    return qtd;
}

int verificarSeEhCoringa(carta retirada)
{
    int testeJoker;

    if (retirada.valor == 14)
    {
        testeJoker = 1;
    }
    else
    {
        testeJoker = 0;
    }

    return testeJoker;
}

int vencedorMaisCartas(areaDeDescarte **a, int qtdeJogadores, jogador *j1, baralhoDesviradoMesa **cartasJogadores, int *maisCartas, int *vencedor, int *empate)
{
    *maisCartas = qtdeCartasMonteDescarte(a[0]);
    *vencedor = 0;
    *empate = 0;
    for (int i = 0; i < qtdeJogadores; i++)
    {
        if (*maisCartas < qtdeCartasMonteDescarte(a[i]))
        {
            *maisCartas = qtdeCartasMonteDescarte(a[i]);
            *vencedor = i;
            *empate = 0;
        }
        if (*maisCartas == qtdeCartasMonteDescarte(a[i]) && *vencedor != i)
        {
            *empate = i;
        }
    }

    for (int i = 0; i < qtdeJogadores; i++)
    {
        while (a[i]->topo != NULL)
        {
            carta carta;
            if (removerAreaDeDescarte(a[i], &carta))
            {
                if (inserirBaralhoMesaOrdenado(cartasJogadores[i], carta))
                {
                }
            }
        }
    }

    for (int i = 0; i < qtdeJogadores; i++)
    {
        printf("\n----------------Jogador %s", j1[i].nome);

        no *aux = cartasJogadores[i]->inicio;
        while (aux != NULL)
        {
            printf("\n[Valor %d, Naipe %d] \n  ", aux->cartaDaVez.valor, aux->cartaDaVez.naipe);

            aux = aux->prox;
        }

        printf("\n\n");
    }

    return 1;
}

int verificarAreaDescarte(areaDeDescarte **a, int qtdeJogadores, carta retirada)
{
    int teste3;
    if (a != NULL)
    {
        for (int x = 0; x < qtdeJogadores; x++)
        {
            if (a[x]->topo != NULL && retirada.valor != 14 && retirada.valor == a[x]->topo->cartaDaVez.valor)
            {
                teste3 = x;
                return teste3;
            }
        }
    }

    teste3 = -1;
    return teste3;
}

int inserirBaralhoMesaOrdenado(baralhoDesviradoMesa *a, carta c)
{

    if (a != NULL)
    {
        no *novo = (no *)malloc(sizeof(no));
        novo->cartaDaVez = c;

        if (a->inicio == NULL)
        {
            novo->prox = NULL;
            a->inicio = novo;
            return 1;
        }
        else
        {
            no *atual = a->inicio, *ant;
            while (atual != NULL && atual->cartaDaVez.valor < novo->cartaDaVez.valor)
            {
                ant = atual;
                atual = atual->prox;
            }

            if (atual == a->inicio)
            {
                novo->prox = a->inicio;
                a->inicio = novo;
            }
            else
            {
                novo->prox = ant->prox;
                ant->prox = novo;
            }
            return 1;
        }
    }
    else
        return 0;
}

baralhoDesviradoMesa **criarMaoFinalJogadores(int qtdeJogadores)
{
    // criando a lista de cartas q vou ordenar
    baralhoDesviradoMesa **cartasJogadores = (baralhoDesviradoMesa **)malloc(qtdeJogadores * sizeof(baralhoDesviradoMesa *));
    for (int f = 0; f < qtdeJogadores; f++)
    {
        cartasJogadores[f] = (baralhoDesviradoMesa *)malloc(sizeof(baralhoDesviradoMesa));
        cartasJogadores[f]->inicio = NULL;
    }

    return cartasJogadores;
}