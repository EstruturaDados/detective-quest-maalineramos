#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Desafio Detective Quest
// Tema 4 - Árvores e Tabela Hash
// Este código inicial serve como base para o desenvolvimento das estruturas de navegação, pistas e suspeitos.
// Use as instruções de cada região para desenvolver o sistema completo com árvore binária, árvore de busca e tabela hash.
#define HASH_SIZE 26

// Estrutura para representar uma sala na mansão
struct sala {
    char nome[30];
    struct sala *esquerda;
    struct sala *direita;
    struct sala *pai;
};

// Estrutura para representar uma pista (para o nível aventureiro)
struct Pista {
    char texto[100];
    struct Pista *esquerda;
    struct Pista *direita;
};

// Estrutura para representar um suspeito (para o nível mestre)
struct Suspeito {
    char nome[50];
    struct Pista *pistas;
    struct Suspeito *proximo; // para tratar colisões
};

// Função para criar uma nova sala
struct sala *criarSala(char nome[]) {
    struct sala *novaSala = malloc(sizeof(struct sala));
    if (!novaSala) {
        fprintf(stderr, "Erro: malloc retornou NULL\n");
        exit(EXIT_FAILURE);
    }
    strncpy(novaSala->nome, nome, sizeof(novaSala->nome) - 1);
    novaSala->nome[sizeof(novaSala->nome) - 1] = '\0';
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    novaSala->pai = NULL;
    return novaSala;
}

// Função para criar uma nova pista
struct Pista* criarPista(char texto[]) {
    struct Pista* nova = malloc(sizeof(struct Pista));
    if (!nova) exit(EXIT_FAILURE);
    strncpy(nova->texto, texto, sizeof(nova->texto)-1);
    nova->texto[sizeof(nova->texto)-1] = '\0';
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}
// Função para inserir uma pista na árvore binária de busca
struct Pista* inserirPista(struct Pista* raiz, struct Pista* nova) {
    if (!raiz) return nova;
    if (strcmp(nova->texto, raiz->texto) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, nova);
    else
        raiz->direita = inserirPista(raiz->direita, nova);
    return raiz;
}

// Função para listar todas as pistas em ordem alfabética
void listarPistas(struct Pista* raiz) {
    if (!raiz) return;
    listarPistas(raiz->esquerda);
    printf("- %s\n", raiz->texto);
    listarPistas(raiz->direita);
}

// fUNção hash 
int hash(char* nome) {
    if (!nome || !isalpha(nome[0])) return 0;
    return toupper(nome[0]) - 'A';
}

void inicializarHash(struct Suspeito* tabela[]) {
    for (int i = 0; i < HASH_SIZE; i++) {
        tabela[i] = NULL;
    }
}

void inserirHash(struct Suspeito* tabela[], char* nomeSuspeito, char* textoPista) {
    int h = hash(nomeSuspeito);
    struct Suspeito* atual = tabela[h];

    while (atual) {
        if (strcmp(atual->nome, nomeSuspeito) == 0) {
            atual->pistas = inserirPista(atual->pistas, criarPista(textoPista));
            return;
        }
        atual = atual->proximo;
    }

    struct Suspeito* novo = malloc(sizeof(struct Suspeito));
    strncpy(novo->nome, nomeSuspeito, sizeof(novo->nome) - 1);
    novo->nome[sizeof(novo->nome) - 1] = '\0';
    novo->pistas = criarPista(textoPista);
    novo->proximo = tabela[h];
    tabela[h] = novo;
}

void listarSuspeitos(struct Suspeito* tabela[]) {
for (int i = 0; i < HASH_SIZE; i++) {
    if (!tabela[i]) continue; // pula se não houver suspeito
    struct Suspeito* atual = tabela[i];
    while (atual) {
            printf("\nSuspeito: %s\n", atual->nome);
            listarPistas(atual->pistas);
            atual = atual->proximo;
        }
    }
}

// Função para listar todas as associações suspeito ↔ pistas
void listarAssociacoes(struct Suspeito* tabela[]) {
    printf("\n🔍 Associações suspeito ↔ pistas:\n");
for (int i = 0; i < HASH_SIZE; i++) {
    if (!tabela[i]) continue; // pula se não houver suspeito
    struct Suspeito* atual = tabela[i];
    while (atual) {
            printf("- %s:\n", atual->nome);
            listarPistas(atual->pistas);
            atual = atual->proximo;
        }
    }
}
// Função para contar o número de pistas em uma árvore
int contarPistas(struct Pista* raiz) {
    if (!raiz) return 0;
    return 1 + contarPistas(raiz->esquerda) + contarPistas(raiz->direita);
}
// Função para verificar se uma pista já existe na árvore
int pistaExiste(struct Pista* raiz, char* texto) {
    if (!raiz) return 0;
    int cmp = strcmp(texto, raiz->texto);
    if (cmp == 0) return 1;
    return pistaExiste(raiz->esquerda, texto) || pistaExiste(raiz->direita, texto);
}


// Função para encontrar o suspeito mais provável baseado no número de pistas associadas
struct Suspeito* suspeitoMaisProvavel(struct Suspeito* tabela[], int* totalPistas) {
    struct Suspeito* maisProvavel = NULL;
    int maxPistas = -1;

    for (int i = 0; i < HASH_SIZE; i++) {
        struct Suspeito* atual = tabela[i];
        while (atual) {
            int count = contarPistas(atual->pistas);  // conta corretamente

            if (count > maxPistas) {
                maxPistas = count;
                maisProvavel = atual;
            }
            atual = atual->proximo;
        }
    }

    if (totalPistas) *totalPistas = maxPistas;
    return maisProvavel;
}


// Função para conectar duas salas a uma sala atual
void conectarSalas(struct sala *atual, struct sala *esq, struct sala *dir) {
    if (!atual) return;
    atual->esquerda = esq;
    atual->direita = dir;
    if (esq) esq->pai = atual;
    if (dir) dir->pai = atual;
}

// Exploração recursiva com loop de validação local
void explorar(struct sala *atual, struct Pista** raizPistas, struct Suspeito* tabela[]) {
    const char* culpadoFinal = "Sr. Black"; // culpado real
    if (atual == NULL) return;

    while (1) {
        printf("\nVocê está na sala: %s\n", atual->nome);

        if (atual->esquerda != NULL) printf("À esquerda: %s\n", atual->esquerda->nome);
        if (atual->direita != NULL) printf("À direita: %s\n", atual->direita->nome);
        if (atual->pai != NULL) printf("Voltar para: %s\n", atual->pai->nome);

        // ---- Pistas automáticas ----
        if (strcmp(atual->nome, "Biblioteca") == 0) {
            if (!pistaExiste(*raizPistas, "Um livro antigo sobre venenos")) {
            *raizPistas = inserirPista(*raizPistas, criarPista("Um livro antigo sobre venenos"));
            inserirHash(tabela, "Sr. Black", "Um livro antigo sobre venenos");
            }
        }
        if (strcmp(atual->nome, "Cozinha") == 0) {
            if (!pistaExiste(*raizPistas, "Frasco quebrado no chão")) {
            *raizPistas = inserirPista(*raizPistas, criarPista("Frasco quebrado no chão"));
            inserirHash(tabela, "Sra. White", "Frasco quebrado no chão");
            }
        }
        if (strcmp(atual->nome, "Sótão") == 0) {
            if (!pistaExiste(*raizPistas, "Fotografia antiga rasgada")) {
            *raizPistas = inserirPista(*raizPistas, criarPista("Fotografia antiga rasgada"));
            inserirHash(tabela, "Coronel Mustard", "Fotografia antiga rasgada");
            }
        }

        char opcao;
        printf("\n📌 Controles:\n(e) esquerda ⬅️\n(d) direita ➡️\n(v) voltar 🔙\n(p) ver pistas 📜\n(h) ver suspeitos 🕵️‍♂️\n(a) acusar 🎯\n(s) sair ❌\nDigite:");
        char linha[10];
        fgets(linha, sizeof(linha), stdin);
        opcao = tolower((unsigned char)linha[0]);


        if (opcao == 's') {
            printf("\nVocê decidiu sair da mansão...\n");
            return;
        } else if (opcao == 'e') {
            if (atual->esquerda != NULL) {
                explorar(atual->esquerda, raizPistas, tabela);
                continue; // Retorna ao loop após explorar
            } else
                printf("\nNão há caminho à esquerda.\n");
        } else if (opcao == 'd') {
            if (atual->direita != NULL) {
                explorar(atual->direita, raizPistas, tabela);
                continue; // Retorna ao loop após explorar
            } else
                printf("\nNão há caminho à direita.\n");
        } else if (opcao == 'v') {
            if (atual->pai != NULL)
                return;
            else
                printf("Você está no Hall — não há onde voltar.\n");
        } else if (opcao == 'p') {
            printf("\n📜 Pistas coletadas (%d):\n", contarPistas(*raizPistas));
            listarPistas(*raizPistas);
        } else if (opcao == 'h') {
            printf("\n🕵️‍♂️ Suspeitos e pistas associadas:\n");
            listarSuspeitos(tabela);
            int total = 0;
            struct Suspeito* provável = suspeitoMaisProvavel(tabela, &total);
            if (provável) {
                printf("\n🎯 Suspeito mais provável até agora: %s (%d pistas)\n", provável->nome, total);
            }

        } else if (opcao == 'a') {
            char chute[50];
            printf("Quem você acusa? ");
            fgets(chute, sizeof(chute), stdin);
            chute[strcspn(chute, "\n")] = 0; // remove \n

            if (strcmp(chute, culpadoFinal) == 0) {
                printf("\n🎉 Parabéns! Você descobriu o culpado: %s!\n", culpadoFinal);
                return; // termina o jogo
            } else {
                printf("\n❌ Erro! %s não é o culpado. Continue investigando...\n", chute);
            }
                       } else {
            printf("Opção inválida.\n");
        }
    }
}

void liberarPistas(struct Pista* raiz) {
    if (!raiz) return;
    liberarPistas(raiz->esquerda);
    liberarPistas(raiz->direita);
    free(raiz);
}

void liberarSuspeitos(struct Suspeito* tabela[]) {
    for (int i=0; i<HASH_SIZE; i++) {
        struct Suspeito* atual = tabela[i];
        while (atual) {
            struct Suspeito* tmp = atual->proximo;
            liberarPistas(atual->pistas);
            free(atual);
            atual = tmp;
        }
    }
}

void liberarSalas(struct sala* raiz) {
    if (!raiz) return;
    liberarSalas(raiz->esquerda);
    liberarSalas(raiz->direita);
    free(raiz);
}


void mostrarCabecalho() {
    // Limpa a tela antes do banner
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    // Cores ANSI
    const char* amarelo = "\033[1;33m";
    const char* ciano = "\033[1;36m";
    const char* vermelho = "\033[1;31m";
    const char* reset = "\033[0m";

    printf("\n%s", amarelo);
    printf("════════════════════════════════════════════════\n");
    printf("             🕵️‍♀️  DETECTIVE QUEST  🕵️‍♂️\n");
    printf("════════════════════════════════════════════════\n");
    
    printf("%s", ciano);
    printf("             🏰 A MANSÃO ESPERA POR VOCÊ 🏰\n");
    printf("        Encontre pistas e descubra o culpado!\n\n");
    
    printf("%s", vermelho);
    printf("     🎯 Níveis:\n");
    printf("        1️⃣ Novato: Exploração de salas\n");
    printf("        2️⃣ Aventureiro: Coleta de pistas\n");
    printf("        3️⃣ Mestre: Suspeitos e pistas\n\n");
    
    printf("%s", amarelo);
    printf("════════════════════════════════════════════════\n\n");
    printf("%s", reset);
}


int main() {
    mostrarCabecalho();  // 🖼 Tela inicial do jogo
    // Inicializaação d
    struct sala *hall = criarSala("Hall de Entrada");
    struct sala *biblioteca = criarSala("Biblioteca");
    struct sala *cozinha = criarSala("Cozinha");
    struct sala *escritorio = criarSala("Escritório");
    struct sala *jardim = criarSala("Jardim");
    struct sala *despensa = criarSala("Despensa");
    struct sala *sotao = criarSala("Sótão");

    // Conexões entre salas
    conectarSalas(hall, biblioteca, cozinha);
    conectarSalas(biblioteca, escritorio, jardim);
    conectarSalas(cozinha, despensa, sotao);

    struct Pista* raizPistas = NULL;
    struct Suspeito* tabela[HASH_SIZE];
    inicializarHash(tabela);   

    explorar(hall, &raizPistas, tabela);

    printf("\nJogo encerrado! Aqui estão suas descobertas finais:\n");
    listarPistas(raizPistas);
    listarSuspeitos(tabela);    
    int total = 0;
    struct Suspeito* culpado = suspeitoMaisProvavel(tabela, &total);
    if (culpado) printf("\n🎯 Suspeito mais provável: %s com %d pistas!\n", culpado->nome, total);

    listarAssociacoes(tabela);

    // Liberação de memória
    liberarSalas(hall);
    liberarPistas(raizPistas);
    liberarSuspeitos(tabela);

    return 0;
}

