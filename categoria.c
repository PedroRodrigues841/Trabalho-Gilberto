#include <stdio.h>
#include <string.h>

#include "categoria.h"
#include "util.h"

enum { CAT_CADASTRAR = 1, CAT_LISTAR, CAT_VOLTAR };

/* ===================== LEITURA DOS CAMPOS ===================== */

static int lerCodigoCategoria(const Categoria lista[], int total) {
    int codigo;

    for (;;) {
        codigo = lerInteiro("Codigo da categoria: ");

        if (!numeroPositivo(codigo)) {
            printf("Codigo invalido! Deve ser um numero positivo.\n");
        } else if (buscarCategoriaPorCodigo(lista, total, codigo) != -1) {
            printf("Codigo ja cadastrado! Informe outro codigo.\n");
        } else {
            return codigo;
        }
    }
}

static void lerNomeCategoria(char nome[], size_t tamanho) {
    for (;;) {
        printf("Nome da categoria: ");
        lerLinha(nome, tamanho);

        if (!textoPreenchido(nome)) {
            printf("Nome invalido! Nao pode ficar em branco.\n");
        } else {
            return;
        }
    }
}

/* ===================== OPERACOES ===================== */

int buscarCategoriaPorCodigo(const Categoria lista[], int total, int codigo) {
    int i;

    for (i = 0; i < total; i++) {
        if (lista[i].codigo == codigo) {
            return i;
        }
    }
    return -1;
}

const char *nomeDaCategoria(const Categoria lista[], int total, int codigo) {
    int posicao = buscarCategoriaPorCodigo(lista, total, codigo);

    if (posicao == -1) {
        return "(categoria desconhecida)";
    }
    return lista[posicao].nome;
}

int cadastrarCategoria(Categoria lista[], int total) {
    Categoria *nova;

    if (total >= MAX_CATEGORIAS) {
        printf("\nLista cheia! Limite de %d categorias atingido.\n",
               MAX_CATEGORIAS);
        return total;
    }

    nova = &lista[total];

    printf("\n----- CADASTRO DE CATEGORIA -----\n");
    nova->codigo = lerCodigoCategoria(lista, total);
    lerNomeCategoria(nova->nome, sizeof(nova->nome));

    printf("\nCategoria cadastrada com sucesso!\n");
    return total + 1;
}

void listarCategorias(const Categoria lista[], int total) {
    int i;

    if (total == 0) {
        printf("\nNenhuma categoria foi cadastrada.\n");
        return;
    }

    printf("\n----- LISTA DE CATEGORIAS (%d) -----\n", total);
    for (i = 0; i < total; i++) {
        printf("Codigo: %d | Nome: %s\n", lista[i].codigo, lista[i].nome);
    }
    printf("-----------------------------------\n");
}

int menuCategorias(Categoria lista[], int total) {
    int opcao;

    do {
        printf("\n===== CATEGORIAS =====\n");
        printf("%d - Cadastrar Categoria\n", CAT_CADASTRAR);
        printf("%d - Listar Categorias\n", CAT_LISTAR);
        printf("%d - Voltar\n", CAT_VOLTAR);
        printf("======================\n");
        opcao = lerInteiro("Escolha uma opcao: ");

        switch (opcao) {
            case CAT_CADASTRAR:
                total = cadastrarCategoria(lista, total);
                break;
            case CAT_LISTAR:
                listarCategorias(lista, total);
                break;
            case CAT_VOLTAR:
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
                break;
        }
    } while (opcao != CAT_VOLTAR);

    return total;
}
