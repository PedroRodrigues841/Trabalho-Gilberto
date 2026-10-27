#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "produto.h"
#include "categoria.h"
#include "util.h"

enum { PESQ_CODIGO = 1, PESQ_NOME };
enum { ESTOQUE_ENTRADA = 1, ESTOQUE_SAIDA };

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

static int lerCategoria(const Categoria categorias[], int totalCategorias) {
    int codigo;

    printf("\nCategorias disponiveis:\n");
    listarCategorias(categorias, totalCategorias);

    for (;;) {
        codigo = lerInteiro("Codigo da categoria: ");

        if (buscarCategoriaPorCodigo(categorias, totalCategorias, codigo)
            == -1) {
            printf("Categoria nao encontrada! Informe um codigo da lista.\n");
        } else {
            return codigo;
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

/* Igual a buscarPorCodigo, mas ignora produtos removidos. */
int buscarAtivoPorCodigo(const Produto lista[], int total, int codigo) {
    int i;

    for (i = 0; i < total; i++) {
        if (lista[i].ativo && lista[i].codigo == codigo) {
            return i;
        }
    }
    return NAO_ENCONTRADO;
}

/* Devolve a posicao do produto ativo na lista ou NAO_ENCONTRADO (-1). */
int buscarPorNome(const Produto lista[], int total, const char nome[]) {
    int i;

    for (i = 0; i < total; i++) {
        if (lista[i].ativo && strcmp(lista[i].nome, nome) == 0) {
            return i;
        }
    }
    return NAO_ENCONTRADO;
}

/* ===================== OPERACOES E TELAS ===================== */

/* Cadastra na proxima posicao livre e devolve o novo total de produtos. */
int cadastrarProduto(Produto lista[], int total,
                     const Categoria categorias[], int totalCategorias) {
    Produto *novo;

    if (totalCategorias == 0) {
        printf("\nNenhuma categoria cadastrada! Cadastre ao menos uma "
               "categoria antes de cadastrar um produto.\n");
        return total;
    }

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
    novo->codigoCategoria = lerCategoria(categorias, totalCategorias);
    novo->disponivel = calcularDisponibilidade(novo->quantidade);
    novo->ativo = 1;

    printf("\n----- RESUMO DO PRODUTO CADASTRADO -----\n");
    exibirProduto(*novo, categorias, totalCategorias);

    return total + 1;
}

void listarProdutos(const Produto lista[], int total,
                    const Categoria categorias[], int totalCategorias) {
    int i;
    int ativos = 0;

    for (i = 0; i < total; i++) {
        if (lista[i].ativo) {
            ativos++;
        }
    }

    if (ativos == 0) {
        printf("\nNenhum produto foi cadastrado.\n");
        return;
    }

    printf("\n----- LISTA DE PRODUTOS (%d) -----\n", ativos);
    for (i = 0; i < total; i++) {
        if (lista[i].ativo) {
            exibirProduto(lista[i], categorias, totalCategorias);
        }
    }
}

void exibirProduto(Produto produto,
                   const Categoria categorias[], int totalCategorias) {
    printf("Codigo................: %d\n", produto.codigo);
    printf("Nome..................: %s\n", produto.nome);
    printf("Categoria.............: %s\n",
           nomeDaCategoria(categorias, totalCategorias,
                           produto.codigoCategoria));
    printf("Prateleira............: %c\n", produto.prateleira);
    printf("Preco unitario........: %.2f\n", produto.preco);
    printf("Quantidade em estoque.: %d\n", produto.quantidade);
    printf("Disponivel para venda.: %s (%d)\n",
           produto.disponivel ? "Sim" : "Nao", produto.disponivel);
    printf("-----------------------------------------\n");
}

void pesquisarProduto(const Produto lista[], int total,
                      const Categoria categorias[], int totalCategorias) {
    char nome[TAM_NOME];
    int tipo;
    int posicao;

    printf("\n----- PESQUISAR PRODUTO -----\n");
    printf("%d - Por codigo\n", PESQ_CODIGO);
    printf("%d - Por nome\n", PESQ_NOME);
    tipo = lerInteiro("Escolha como pesquisar: ");

    switch (tipo) {
        case PESQ_CODIGO:
            posicao = buscarAtivoPorCodigo(lista, total, lerInteiro("Codigo: "));
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
        exibirProduto(lista[posicao], categorias, totalCategorias);
    }
}

/* ===================== ESTOQUE E REMOCAO ===================== */

void alterarEstoque(Produto lista[], int total) {
    int posicao;
    int tipo;
    int quantidade;
    Produto *p;

    printf("\n----- ALTERAR ESTOQUE -----\n");
    posicao = buscarAtivoPorCodigo(lista, total, lerInteiro("Codigo: "));

    if (posicao == NAO_ENCONTRADO) {
        printf("\nNenhum produto foi encontrado.\n");
        return;
    }

    p = &lista[posicao];
    printf("Produto: %s | Estoque atual: %d\n", p->nome, p->quantidade);
    printf("%d - Entrada (chegada de mercadoria)\n", ESTOQUE_ENTRADA);
    printf("%d - Saida (venda ou retirada)\n", ESTOQUE_SAIDA);
    tipo = lerInteiro("Escolha o tipo de movimentacao: ");

    if (tipo != ESTOQUE_ENTRADA && tipo != ESTOQUE_SAIDA) {
        printf("\nOpcao invalida! Nada foi alterado.\n");
        return;
    }

    for (;;) {
        quantidade = lerInteiro("Quantidade: ");
        if (!numeroPositivo(quantidade)) {
            printf("Quantidade invalida! Deve ser maior que zero.\n");
        } else {
            break;
        }
    }

    if (tipo == ESTOQUE_ENTRADA) {
        if (quantidade > INT_MAX - p->quantidade) {
            printf("\nQuantidade final fora do limite permitido. "
                   "Nada foi alterado.\n");
            return;
        }
        p->quantidade += quantidade;
    } else {
        if (quantidade > p->quantidade) {
            printf("\nSaida maior que o estoque atual (%d)! "
                   "Operacao nao realizada.\n", p->quantidade);
            return;
        }
        p->quantidade -= quantidade;
    }

    p->disponivel = calcularDisponibilidade(p->quantidade);

    printf("\nEstoque atualizado! Quantidade em estoque: %d | "
           "Disponivel para venda: %s (%d)\n",
           p->quantidade, p->disponivel ? "Sim" : "Nao", p->disponivel);
}

void removerProduto(Produto lista[], int total) {
    int posicao;

    printf("\n----- REMOVER PRODUTO -----\n");
    posicao = buscarAtivoPorCodigo(lista, total, lerInteiro("Codigo: "));

    if (posicao == NAO_ENCONTRADO) {
        printf("\nNenhum produto foi encontrado.\n");
        return;
    }

    lista[posicao].ativo = 0;
    printf("\nProduto \"%s\" removido com sucesso.\n", lista[posicao].nome);
}
