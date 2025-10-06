#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Desafio Detective Quest
// Tema 4 - Árvores e Tabela Hash
// Este código inicial serve como base para o desenvolvimento das estruturas de navegação, pistas e suspeitos.
// Use as instruções de cada região para desenvolver o sistema completo com árvore binária, árvore de busca e tabela hash.

// Estrutura para representar uma sala na mansão
struct sala {
    char nome[30];
    struct sala *esquerda;
    struct sala *direita;
    struct sala *pai;
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

// Função para conectar duas salas a uma sala atual
void conectarSalas(struct sala *atual, struct sala *esq, struct sala *dir) {
    if (!atual) return;
    atual->esquerda = esq;
    atual->direita = dir;
    if (esq) esq->pai = atual;
    if (dir) dir->pai = atual;
}

// Exploração recursiva com loop de validação local
void explorar(struct sala *atual) {
    if (atual == NULL) return;

    while (1) {
        printf("\nVocê está na sala: %s\n", atual->nome);

        if (atual->esquerda != NULL) printf("À esquerda: %s\n", atual->esquerda->nome);
        if (atual->direita != NULL) printf("À direita: %s\n", atual->direita->nome);
        if (atual->pai != NULL) printf("Voltar para: %s\n", atual->pai->nome);

        char opcao;
        printf("Ir para (e) esquerda, (d) direita, (v) voltar, ou (s) sair: ");
        if (scanf(" %c", &opcao) != 1) {
            // limpa entrada e tenta de novo
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            printf("Entrada inválida. Tente novamente.\n");
            continue;
        }

        opcao = tolower((unsigned char)opcao);

        if (opcao == 's') {
            printf("Você decidiu sair da mansão...\n");
            return;
        } else if (opcao == 'e') {
            if (atual->esquerda != NULL) {
                explorar(atual->esquerda); // desce para a esquerda (recursivo)
                // quando retornar, continua aqui (voltou)
            } else {
                printf("Não há caminho à esquerda. Escolha outra opção.\n");
            }
        } else if (opcao == 'd') {
            if (atual->direita != NULL) {
                explorar(atual->direita); // desce para a direita (recursivo)
            } else {
                printf("Não há caminho à direita. Escolha outra opção.\n");
            }
        } else if (opcao == 'v') {
            if (atual->pai != NULL) {
                // chama recursivamente para o pai — alternativa: simplesmente 'return' e
                // permitir que a chamada anterior (que entrou nesta sala) continue.
                return; // sobe para a chamada anterior que representa a sala pai
            } else {
                printf("Você está no nó raiz; não há onde voltar.\n");
            }
        } else {
            printf("Opção inválida. Use e/d/v/s.\n");
        }
        // aqui o loop repete, sem empilhar chamadas inúteis
    }
}


int main() {

    // 🌱 Nível Novato: Mapa da Mansão com Árvore Binária
    //
    // - Crie uma struct Sala com nome, e dois ponteiros: esquerda e direita.
    // - Use funções como criarSala(), conectarSalas() e explorarSalas().
    // - A árvore pode ser fixa: Hall de Entrada, Biblioteca, Cozinha, Sótão etc.
    // - O jogador deve poder explorar indo à esquerda (e) ou à direita (d).
    // - Finalize a exploração com uma opção de saída (s).
    // - Exiba o nome da sala a cada movimento.
    // - Use recursão ou laços para caminhar pela árvore.
    // - Nenhuma inserção dinâmica é necessária neste nível.
        // Criação das salas
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

    explorar(hall);

    free(hall);
    free(biblioteca);
    free(cozinha);
    free(escritorio);
    free(jardim);
    free(despensa);
    free(sotao);
    // 🔍 Nível Aventureiro: Armazenamento de Pistas com Árvore de Busca
    //
    // - Crie uma struct Pista com campo texto (string).
    // - Crie uma árvore binária de busca (BST) para inserir as pistas coletadas.
    // - Ao visitar salas específicas, adicione pistas automaticamente com inserirBST().
    // - Implemente uma função para exibir as pistas em ordem alfabética (emOrdem()).
    // - Utilize alocação dinâmica e comparação de strings (strcmp) para organizar.
    // - Não precisa remover ou balancear a árvore.
    // - Use funções para modularizar: inserirPista(), listarPistas().
    // - A árvore de pistas deve ser exibida quando o jogador quiser revisar evidências.

    // 🧠 Nível Mestre: Relacionamento de Pistas com Suspeitos via Hash
    //
    // - Crie uma struct Suspeito contendo nome e lista de pistas associadas.
    // - Crie uma tabela hash (ex: array de ponteiros para listas encadeadas).
    // - A chave pode ser o nome do suspeito ou derivada das pistas.
    // - Implemente uma função inserirHash(pista, suspeito) para registrar relações.
    // - Crie uma função para mostrar todos os suspeitos e suas respectivas pistas.
    // - Adicione um contador para saber qual suspeito foi mais citado.
    // - Exiba ao final o “suspeito mais provável” baseado nas pistas coletadas.
    // - Para hashing simples, pode usar soma dos valores ASCII do nome ou primeira letra.
    // - Em caso de colisão, use lista encadeada para tratar.
    // - Modularize com funções como inicializarHash(), buscarSuspeito(), listarAssociacoes().

    return 0;
}

