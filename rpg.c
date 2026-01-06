#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct Sala {
    int id;
    char descricao[255];
    int temInimigo;      
    int chefe;
    int temTesouro;     
    int mimico;
    struct Sala *esquerda;  
    struct Sala *direita;   
    struct Sala *pai;       
} Sala;

typedef struct Entidade {
    int vida;
    int dano;
    int esforco;
} Entidade;

Sala* criarSala(int id, Sala* pai) {
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));
    novaSala->id = id;
    strcpy(novaSala->descricao, "Uma sala suja feita de tijolos, voce nao encontra nada de interessante.");
    novaSala->temInimigo = rand() % 2;
    if (novaSala->temInimigo) {
        novaSala->chefe = (rand() % 4 == 0);
    }
    novaSala->temTesouro = rand() % 2;
    novaSala->mimico = 0;
    if (novaSala->temTesouro) {
        novaSala->mimico = rand() % 2;
    }
    novaSala->pai = pai;
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

Sala* gerarMasmorra(Sala* pai, int nivelAtual, int nivelMaximo, int *contadorId) {
    if (nivelAtual >= nivelMaximo) {
        return NULL;
    }

    Sala* sala = criarSala((*contadorId)++, pai);

    if (sala->temTesouro) {
        if (sala->mimico) {
            strcpy(sala->descricao, "Um baú luxuoso ornamentado com ouro. Estranhamente, ele parece tremer como se estivesse ansioso para ser aberto.");
        } else {
            strcpy(sala->descricao, "Um baú luxuoso ornamentado com ouro.");
        }
    }
    
    if (sala->temInimigo) {
        if (sala->chefe) {
            strcpy(sala->descricao, "Você encontra um inimigo forte. Tenha cuidado.");
        } else {
            strcpy(sala->descricao, "Você encontra um inimigo comum.");
        }
    } 

    if (sala->temInimigo && sala->temTesouro) {
        if (sala->mimico) {
            strcpy(sala->descricao, "Voce encontra um inimigo protegendo um bau que parece esta se tremendo");
        } else {
            strcpy(sala->descricao, "Voce encontra um inimigo protegendo um bau");
        } 
    }

    if ((rand() % 100) < 80) {
        sala->esquerda = gerarMasmorra(sala, nivelAtual + 1, nivelMaximo, contadorId);
    }
    
    if ((rand() % 100) < 80) {
        sala->direita = gerarMasmorra(sala, nivelAtual + 1, nivelMaximo, contadorId);
    }

    return sala;
}

int contarSalas(Sala *raiz) {
    if (raiz == NULL) {
        return 0;
    }

    return 1 + contarSalas(raiz->esquerda) + contarSalas(raiz->direita);
}

void liberarMasmorra(Sala *raiz) {
    if (raiz == NULL) return;
    liberarMasmorra(raiz->esquerda);
    liberarMasmorra(raiz->direita);
    free(raiz);
}

int menorProfundidade(Sala *raiz) {
    if (raiz == NULL) return 0;

    if (raiz->esquerda == NULL && raiz->direita == NULL) return 1;

    if (raiz->esquerda == NULL) 
        return menorProfundidade(raiz->direita) + 1;

    if (raiz->direita == NULL) 
        return menorProfundidade(raiz->esquerda) + 1;

    int esq = menorProfundidade(raiz->esquerda);
    int dir = menorProfundidade(raiz->direita);

    if (esq < dir) return esq + 1;
    else return dir + 1;
}

void iniciarJogo(Sala *raiz) {
    if (raiz == NULL) {
        printf("A masmorra está vazia! Nao ha nada para explorar.\n");
        return;
    }

    Sala *salaAtual = raiz;
    Entidade jogador;
    jogador.vida = 20;
    jogador.dano = 5;
    jogador.esforco = 10;
    int escolha;

    printf("--- BEM-VINDO A MASMORRA DA ARVORE ---\n");

    while (salaAtual != NULL) {
        printf("\n==================================\n");
        printf("Voce esta na Sala ID: %d\n", salaAtual->id);
        printf("vida: %d | dano: %d | esforco: %d\n", jogador.vida, jogador.dano, jogador.esforco);
        
        printf("\n%s\n", salaAtual->descricao);
        if (salaAtual->temInimigo) {
            Entidade inimigo;
            if (salaAtual->chefe) {
                inimigo.vida = 15;
                inimigo.dano = 4;
            } else {
                inimigo.vida = 10;
                inimigo.dano = 2;
            }
            while (inimigo.vida > 0) {
                printf("\n==================COMBATE=======================\n");
                printf("vida: %d | dano: %d | esforco: %d\n", jogador.vida, jogador.dano, jogador.esforco);
                printf("inimigo - vida: %d | dano: %d\n", inimigo.vida, inimigo.dano);
                printf("\n1 - ataque rapido (dano: %d)\n", jogador.dano);
                printf("2 - ataque forte (dano: %d)\n", jogador.dano*2);

                printf("Escolha: ");
                scanf("%d", &escolha);
                if (escolha == 1) {
                    inimigo.vida = inimigo.vida - jogador.dano;
                } else if (escolha == 2 && jogador.esforco > 0) {
                    inimigo.vida = inimigo.vida - jogador.dano*2;
                    jogador.esforco = jogador.esforco - 2;
                } else {
                    printf("\nOpcao inexistente!\n");
                }

                jogador.vida = jogador.vida - inimigo.dano;
                if (jogador.vida <= 0) {
                    printf("\nFim de jogo.\nVoce morreu faltando %d salas para o fim da masmorra.\n", menorProfundidade(salaAtual));
                    return;
                }
            }
            printf("\n==========Voce derrotou o inimigo!========\n");
            printf("Voce esta na Sala ID: %d\n", salaAtual->id);
            salaAtual->temInimigo = 0;
            salaAtual->chefe = 0;
            strcpy(salaAtual->descricao, "Uma sala suja feita de tijolos, voce nao encontra nada de interessante.");
        }
        
        if (salaAtual->temTesouro) {
            printf("O bau esta na sua frente. Deseja abri-lo?\n");
            printf("\n1 - Sim\n");
            printf("2 - Nao\n");
            printf("Escolha: ");
            scanf("%d", &escolha);
            if (salaAtual->mimico && escolha == 1) {
                printf("\nO bau cria dentes e morde sua mao!\nVoce perde 3 pontos de vida.\n");
                jogador.vida = jogador.vida - 3;
                if (jogador.vida <= 0) {
                    printf("\nVoce morreu faltando %d salas para o fim da masmorra.\n", menorProfundidade(salaAtual));
                    return;
                }
            } else if (escolha == 1) {
                int item = (rand() % 3) + 1;
                if (item == 1) {
                    printf("\nPocao de vitalidade: +5 de vida\n");
                    jogador.vida = jogador.vida + 5;
                } else if (item == 2) {
                    printf("\nRuna de dano: +2 de dano\n");
                    jogador.dano = jogador.dano + 2;
                } else {
                    printf("\nAgua potavel: +2 de esforco\n");
                    jogador.esforco = jogador.esforco + 2;
                }
            }
            printf("Voce esta na Sala ID: %d\n", salaAtual->id);
            salaAtual->temTesouro = 0;
            strcpy(salaAtual->descricao, "Uma sala suja feita de tijolos, voce nao encontra nada de interessante.");
        }

        if (salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
            printf("\nVoce chegou ao final da masmorra.\n");
            break;
        }

        printf("\nPara onde deseja ir?\n");
        
        if (salaAtual->esquerda != NULL) 
            printf("1 - Seguir pelo tunel da ESQUERDA (Sala %d)\n", salaAtual->esquerda->id);
        else 
            printf("1 - [Caminho da esquerda bloqueado]\n");

        if (salaAtual->direita != NULL) 
            printf("2 - Seguir pelo tunel da DIREITA (Sala %d)\n", salaAtual->direita->id);
        else 
            printf("2 - [Caminho da direita bloqueado]\n");

        if (salaAtual->pai != NULL) {
            printf("3 - VOLTAR para Sala %d\n", salaAtual->pai->id);
        } else {
            printf("3 - SAIR da Masmorra (Voce esta na entrada)\n");
        }

        printf("Escolha: ");
        scanf("%d", &escolha);

        if (escolha == 1 && salaAtual->esquerda != NULL) {
            salaAtual = salaAtual->esquerda;
        } 
        else if (escolha == 2 && salaAtual->direita != NULL) {
            salaAtual = salaAtual->direita;
        } 
        else if (escolha == 3) {
            if (salaAtual->pai != NULL) {
                printf("\nVoce retorna de onde veio.\n");
                salaAtual = salaAtual->pai;
            } else {
                printf("\nVoce decidiu sair da masmorra.\n");
                break;
            }
        } 
        else {
            printf("\n[Caminho invalido ou sem saida!]\n");
        }
    }

    printf("Fim da aventura.\n");
}

int main() {
    srand(time(NULL));
    int contadorIds = 1;
    
    int profundidade = 0;
    while (profundidade < 4) {
        printf("O quao funda deseja que seja a masmorra? (recomendado: 4 a 7)\n");
        printf("Escolha: ");
        scanf("%d", &profundidade);
        if (profundidade < 4) {
            printf("Masmorra pequena demais!\n");
        } else if (profundidade > 10) {
            printf("Masmorra grande demais!\n");
        } else {
            printf("\n");
        }
    }

    Sala* masmorra = gerarMasmorra(NULL, 0, profundidade, &contadorIds);

    int total = contarSalas(masmorra);
    printf("Masmorra gerada com %d salas.\n", total);

    iniciarJogo(masmorra);

    liberarMasmorra(masmorra);

    return 0;
}