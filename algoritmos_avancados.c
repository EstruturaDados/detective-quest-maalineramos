#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Desafio Detective Quest
// Tema 4 - Árvores e Tabela Hash
// Este código inicial serve como base para o desenvolvimento das estruturas de navegação, pistas e suspeitos.
// Use as instruções de cada região para desenvolver o sistema completo com árvore binária, árvore de busca e tabela hash.

// Estrutura para representar uma sala na mansão
struct sala {
    char nome[30];
    struct sala *esquerda;
    struct sala *direita;
};

// Função para criar uma nova sala
struct sala *criarSala(char nome[]) {
    struct sala *novaSala = (struct sala *)malloc(sizeof(struct sala));
    strcpy(novaSala->nome, nome);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

// Função para conectar duas salas a uma sala atual
struct sala *conectarSalas(struct sala *esquerda, struct sala *direita, struct sala *atual) {
    atual->esquerda = esquerda;
    atual->direita = direita;
    return atual;
}

void explorar(struct sala *atual) {
    char opcao;

    while (1) {
        printf("\nVocê está na sala: %s\n", atual->nome);
        printf("Ir para (e) esquerda, (d) direita, ou (s) sair: ");
        scanf(" %c", &opcao);

        if (opcao == 's') {
            printf("Você decidiu sair da mansão...\n");
            break;
        } else if (opcao == 'e' && atual->esquerda != NULL) {
            atual = atual->esquerda;
        } else if (opcao == 'd' && atual->direita != NULL) {
            atual = atual->direita;
        } else {
            printf("Caminho bloqueado! Escolha outro.\n");
        }
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

    // Início da exploração
    // Conexões (árvore binária fixa)
    hall->esquerda = biblioteca;
    hall->direita = cozinha;
    biblioteca->esquerda = escritorio;
    biblioteca->direita = jardim;
    cozinha->esquerda = despensa;
    cozinha->direita = sotao;

    explorar(hall);
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

