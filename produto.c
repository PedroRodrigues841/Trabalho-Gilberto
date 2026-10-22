#include <stdio.h>
#include <string.h>

#include "produto.h"
#include "util.h"

enum { PESQ_CODIGO = 1, PESQ_NOME };

/* ===================== LEITURA DOS CAMPOS ===================== */

static int lerCodigo(const Produto lista[], int total) {
    int codigo;

    for (;;) {
        codigo = lerInteiro("Codigo: ");

        if (!numeroPositivo(codigo)) {
            printf("Codigo invalido! Deve ser um numero positivo.\n");
        } else if (buscarPorCodigo(lista, total, codigo) != NAO_ENCONTRADO) {
            printf("Codigo ja cadastrado! Informe outro codigo.\n");
        } else {
            return codigo;
        }
    }
}

static void lerNome(char nome[], size_t tamanho) {
    for (;;) {
        printf("Nome: ");
        lerLinha(nome, tamanho);

        if (!textoPreenchido(nome)) {
            printf("Nome invalido! Nao pode ficar em branco.\n");
        } else {
            return;
        }
    }
}

static char lerPrateleira(void) {
    char linha[64];
    char prateleira;

    for (;;) {
        printf("Prateleira (%c a %c): ", PRATELEIRA_MIN, PRATELEIRA_MAX);
        lerLinha(linha, sizeof(linha));

        if (extrairLetraUnica(linha, &prateleira) &&
            letraNoIntervalo(prateleira, PRATELEIRA_MIN, PRATELEIRA_MAX)) {
            return prateleira;
        }
        printf("Prateleira invalida! Use apenas uma letra de %c a %c.\n",
               PRATELEIRA_MIN, PRATELEIRA_MAX);
    }
}

static float lerPreco(void) {
    float preco;

    for (;;) {
        preco = lerFloat("Preco unitario: ");

        if (!numeroPositivo(preco)) {
            printf("Preco invalido! Deve ser maior que zero.\n");
        } else {
            return preco;
        }
    }
}

static int lerQuantidade(void) {
    int quantidade;

    for (;;) {
        quantidade = lerInteiro("Quantidade em estoque: ");

        if (!numeroNaoNegativo(quantidade)) {
            printf("Quantidade invalida! Nao pode ser negativa.\n");
        } else {
            return quantidade;
        }
    }
}

/* ===================== REGRAS DO PRODUTO ===================== */

int lerOpcaoMenu(void) {
    return lerInteiro("Escolha uma opcao: ");
}

int calcularDisponibilidade(int quantidade) {
    return (quantidade > 0) ? 1 : 0;
}

/* Devolve a posicao do produto na lista ou NAO_ENCONTRADO (-1). */
int buscarPorCodigo(const Produto lista[], int total, int codigo) {
    int i;

    for (i = 0; i < total; i++) {
        if (lista[i].codigo == codigo) {
            return i;
        }
    }
    return NAO_ENCONTRADO;
}

/* Devolve a posicao do produto na lista ou NAO_ENCONTRADO (-1). */
int buscarPorNome(const Produto lista[], int total, const char nome[]) {
    int i;

    for (i = 0; i < total; i++) {
        if (strcmp(lista[i].nome, nome) == 0) {
            return i;
        }
    }
    return NAO_ENCONTRADO;
}

/* ===================== OPERACOES E TELAS ===================== */

/* Cadastra na proxima posicao livre e devolve o novo total de produtos. */
int cadastrarProduto(Produto lista[], int total) {
    Produto *novo;

    if (total >= MAX_PRODUTOS) {
        printf("\nLista cheia! Limite de %d produtos atingido.\n", MAX_PRODUTOS);
        return total;
    }

    novo = &lista[total];

    printf("\n----- CADASTRO DE PRODUTO -----\n");
    novo->codigo = lerCodigo(lista, total);
    lerNome(novo->nome, sizeof(novo->nome));
    novo->prateleira = lerPrateleira();
    novo->preco = lerPreco();
    novo->quantidade = lerQuantidade();
    novo->disponivel = calcularDisponibilidade(novo->quantidade);

    printf("\n----- RESUMO DO PRODUTO CADASTRADO -----\n");
    exibirProduto(*novo);

    return total + 1;
}

void listarProdutos(const Produto lista[], int total) {
    int i;

    if (total == 0) {
        printf("\nNenhum produto foi cadastrado.\n");
        return;
    }

    printf("\n----- LISTA DE PRODUTOS (%d) -----\n", total);
    for (i = 0; i < total; i++) {
        exibirProduto(lista[i]);
    }
}

void exibirProduto(Produto produto) {
    printf("Codigo................: %d\n", produto.codigo);
    printf("Nome..................: %s\n", produto.nome);
    printf("Prateleira............: %c\n", produto.prateleira);
    printf("Preco unitario........: %.2f\n", produto.preco);
    printf("Quantidade em estoque.: %d\n", produto.quantidade);
    printf("Disponivel para venda.: %s (%d)\n",
           produto.disponivel ? "Sim" : "Nao", produto.disponivel);
    printf("-----------------------------------------\n");
}

void pesquisarProduto(const Produto lista[], int total) {
    char nome[TAM_NOME];
    int tipo;
    int posicao;

    printf("\n----- PESQUISAR PRODUTO -----\n");
    printf("%d - Por codigo\n", PESQ_CODIGO);
    printf("%d - Por nome\n", PESQ_NOME);
    tipo = lerInteiro("Escolha como pesquisar: ");

    switch (tipo) {
        case PESQ_CODIGO:
            posicao = buscarPorCodigo(lista, total, lerInteiro("Codigo: "));
            break;
        case PESQ_NOME:
            lerNome(nome, sizeof(nome));
            posicao = buscarPorNome(lista, total, nome);
            break;
        default:
            printf("\nOpcao de pesquisa invalida!\n");
            return;
    }

    if (posicao == NAO_ENCONTRADO) {
        printf("\nNenhum produto foi encontrado.\n");
    } else {
        printf("\n----- PRODUTO ENCONTRADO -----\n");
        exibirProduto(lista[posicao]);
    }
}
