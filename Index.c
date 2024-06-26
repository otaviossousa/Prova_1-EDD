#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Definição da estrutura do nó da árvore
 *
 * 1 - Cada nó da árvore deve representar um produto, contendo os seguintes campos: código do produto, nome do produto,
 * quantidade em estoque, preço unitário.*/
typedef struct Node {
    int codigo;
    char nome[50];
    int quantidade;
    float preco;
    struct Node* esquerda;
    struct Node* direita;
} Node;

// Função para criar nó
Node* criarNo(int codigo, const char *nome, int quantidade, float preco) {
    if (codigo < 0 || quantidade < 0 || preco < 0) {
        printf("Nao e possivel criar o no com valores negativos.\n");
        exit(EXIT_FAILURE);
    }

    Node* novoNo = (Node*)calloc(1, sizeof(Node)); // Inicializa a memória alocada com zero
    if (novoNo == NULL) {
        printf("Erro na alocacao de memoria.\n");
        exit(EXIT_FAILURE);
    }

    novoNo->codigo = codigo;
    strcpy(novoNo->nome, nome);
    novoNo->quantidade = quantidade;
    novoNo->preco = preco;
    novoNo->esquerda = NULL;
    novoNo->direita = NULL;
    return novoNo;
}

/* Função para inserir elementos na árvore
 *
 * 2 - Implemente uma função para inserir um novo produto na árvore, mantendo-a balanceada. Garanta que a árvore
 * não esteja desbalanceada em mais de 1 nível.
 *
 * 8 - Garanta que não é possível inserir produtos com códigos duplicados na árvore, nem quantidades ou
 * preços menores que zero.*/
Node* inserir(Node* raiz, int codigo, const char *nome, int quantidade, float preco) {
    if (codigo < 0 || quantidade < 0 || preco < 0) {
        printf("Nao e possivel inserir um produto com valores negativos.\n");
        return raiz; // Retorna a raiz original sem inserir o produto
    }

    if (raiz == NULL) {
        raiz = criarNo(codigo, nome, quantidade, preco);
        printf("Produto inserido com sucesso!\n");
        return raiz;
    }

    if (codigo == raiz->codigo) {
        printf("Produto com codigo %d ja existe na arvore.\n", codigo);
        return raiz; // Retorna a raiz original sem inserir o produto duplicado
    }

    if (codigo < raiz->codigo) {
        raiz->esquerda = inserir(raiz->esquerda, codigo, nome, quantidade, preco);
    } else if (codigo > raiz->codigo) {
        raiz->direita = inserir(raiz->direita, codigo, nome, quantidade, preco);
    }
    // Balanceamento da árvore (a ser implementado)
    return raiz;
}

/* Função para remover um elemento da árvore
 *
 * 3 - Implemente uma função para remover um produto da árvore, atualizando a estrutura para manter
 * a propriedade da árvore binária.*/
Node* remover(Node* raiz, int chave) {
    if (raiz == NULL) {
        printf("Valor nao encontrado!\n");
        return NULL;
    } else {
        if (raiz->codigo == chave) {
            // Remove nós folhas
            if (raiz->esquerda == NULL && raiz->direita == NULL) {
                printf("Removendo o produto: Codigo: %d, Nome: %s, Quantidade: %d, Preco: %.2f\n", raiz->codigo, raiz->nome, raiz->quantidade, raiz->preco);
                free(raiz);
                return NULL;
            } else {
                // Remove nós que possuem apenas 1 filho
                if (raiz->esquerda == NULL || raiz->direita == NULL) {
                    Node *aux;
                    if (raiz->esquerda != NULL)
                        aux = raiz->esquerda;
                    else
                        aux = raiz->direita;
                    printf("Removendo o produto: Codigo: %d, Nome: %s, Quantidade: %d, Preco: %.2f\n", raiz->codigo, raiz->nome, raiz->quantidade, raiz->preco);
                    free(raiz);
                    return aux;
                } else {
                    // Remove nós com dois filhos
                    Node* aux = raiz->esquerda;
                    while (aux->direita != NULL)
                        aux = aux->direita;
                    raiz->codigo = aux->codigo;
                    strcpy(raiz->nome, aux->nome); // Atualiza o nome
                    raiz->quantidade = aux->quantidade; // Atualiza a quantidade
                    raiz->preco = aux->preco; // Atualiza o preço
                    raiz->esquerda = remover(raiz->esquerda, aux->codigo); // Remove o nó sucessor
                    return raiz;
                }
            }
        } else {
            if (chave < raiz->codigo)
                raiz->esquerda = remover(raiz->esquerda, chave);
            else
                raiz->direita = remover(raiz->direita, chave);
            return raiz;
        }
    }
    return NULL;
}

/* Função para buscar um produto pelo código
 *
 * 4 Implemente uma função para buscar um produto na árvore pelo seu código.*/
Node* buscar(Node* raiz, int codigo) {
    if (raiz == NULL || raiz->codigo == codigo) {
        return raiz;
    }

    if (codigo < raiz->codigo) {
        return buscar(raiz->esquerda, codigo);
    } else {
        return buscar(raiz->direita, codigo);
    }
}

/* Função para listar produtos com quantidade menor que um valor especificado
 *
 * 5 - Implemente uma função para listar todos os produtos com uma quantidade menor que a informada pelo usuário.*/
void listarQuantidadeMenor(Node* raiz, int valor) {
    if (raiz == NULL) {
        return;
    }

    listarQuantidadeMenor(raiz->esquerda, valor);
    if (raiz->quantidade < valor) {
        printf("Codigo: %d, Nome: %s, Quantidade: %d, Preco: %.2f\n", raiz->codigo, raiz->nome, raiz->quantidade, raiz->preco);
    }
    listarQuantidadeMenor(raiz->direita, valor);
}

/* Função para listar produtos com preço dentro de uma faixa especificada
 *
 * 6 - Implemente uma função para listar todos os produtos em uma faixa de preço especificada pelo usuário.*/
void listarFaixaPreco(Node* raiz, float minPreco, float maxPreco) {
    if (raiz == NULL) {
        return;
    }

    listarFaixaPreco(raiz->esquerda, minPreco, maxPreco);
    if (raiz->preco >= minPreco && raiz->preco <= maxPreco) {
        printf("Codigo: %d, Nome: %s, Quantidade: %d, Preco: %.2f\n", raiz->codigo, raiz->nome, raiz->quantidade, raiz->preco);
    }
    listarFaixaPreco(raiz->direita, minPreco, maxPreco);
}

/* Função para calcular o valor total do estoque
 *
 * 7 - Implemente uma função para calcular o valor total do estoque da loja.*/
float calcularValorTotal(Node* raiz) {
    if (raiz == NULL) {
        return 0;
    }

    return raiz->preco * raiz->quantidade + calcularValorTotal(raiz->esquerda) + calcularValorTotal(raiz->direita);
}

/* Função para imprimir a árvore
 *
 * 10 - O código deve implementar uma maneira de exibir a árvore de maneira intuitiva no console.*/
void imprimirArvore(Node* raiz, int espacos) {
    if (raiz == NULL) {
        return;
    }

    espacos += 10;

    imprimirArvore(raiz->direita, espacos);

    printf("\n");
    for (int i = 10; i < espacos; i++) {
        printf(" ");
    }
    printf("%d - %s\n", raiz->codigo, raiz->nome);

    imprimirArvore(raiz->esquerda, espacos);
}

int main() {
    Node* raiz = NULL;
    int opcao;

    do {
        printf("\n=== Menu ===\n");
        printf("0. Sair\n");
        printf("1. Inserir produto\n");
        printf("2. Remover produto\n");
        printf("3. Buscar produto\n");
        printf("4. Listar produtos com preco dentro de uma faixa\n");
        printf("5. Listar produtos com quantidade menor que um valor especificado\n");
        printf("6. Calcular valor total do estoque\n");
        printf("7. Imprimir arvore de produtos\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 0:
                printf("Saindo...\n");
                break;
            case 1: {
                int codigo, quantidade;
                float preco;
                char nome[50];
                printf("\nDigite o codigo do produto:");
                scanf("%d", &codigo);
                printf("Digite o nome do produto:");
                scanf("%s", nome);
                printf("Digite a quantidade do produto:");
                scanf("%d", &quantidade);
                printf("Digite o preco do produto:");
                scanf("%f", &preco);
                raiz = inserir(raiz, codigo, nome, quantidade, preco);
                break;
            }
            case 2: {
                int codigoRemover;
                printf("\nDigite o codigo do produto a ser removido:");
                scanf("%d", &codigoRemover);
                raiz = remover(raiz, codigoRemover);
                break;
            }
            case 3: {
                int codigoBuscar;
                printf("\nDigite o codigo do produto a ser buscado:");
                scanf("%d", &codigoBuscar);
                Node* produtoBuscado = buscar(raiz, codigoBuscar);
                if (produtoBuscado != NULL) {
                    printf("Produto encontrado:\n");
                    printf("Codigo: %d, Nome: %s, Quantidade: %d, Preco: R$ %.2f\n", produtoBuscado->codigo, produtoBuscado->nome, produtoBuscado->quantidade, produtoBuscado->preco);
                } else {
                    printf("Produto com codigo %d nao encontrado!\n", codigoBuscar);
                }
                break;
            }
            case 4: {
                float minPreco, maxPreco;
                printf("\nDigite o preco minimo:");
                scanf("%f", &minPreco);
                printf("Digite o preco maximo:");
                scanf("%f", &maxPreco);
                printf("Produtos com preco entre R$ %.2f e R$ %.2f:\n", minPreco, maxPreco);
                listarFaixaPreco(raiz, minPreco, maxPreco);
                break;
            }
            case 5: {
                int valor;
                printf("\nDigite a quantidade maxima:");
                scanf("%d", &valor);
                printf("Produtos com quantidade menor que %d:\n", valor);
                listarQuantidadeMenor(raiz, valor);
                break;
            }
            case 6:
                printf("\nValor total do estoque: R$ %.2f\n", calcularValorTotal(raiz));
                break;
            case 7:
                printf("\nArvore de produtos:\n");
                imprimirArvore(raiz, 0);
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 0);

    return 0;
}