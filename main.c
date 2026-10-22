#include <stdio.h>
#include <stdlib.h>

#include "produto.h"
#include "categoria.h"
#include "arquivo.h"

enum { OPC_CADASTRAR = 1, OPC_LISTAR, OPC_PESQUISAR, OPC_CATEGORIAS, OPC_SAIR };

void exibirMenu(void);

int main(void) {
    Categoria categorias[MAX_CATEGORIAS];
    Produto produtos[MAX_PRODUTOS];
    int totalCategorias;
    int total;
    int opcao;

    /* Carrega os dados salvos (lista vazia se os arquivos nao existirem). */
    totalCategorias = carregarCategorias(categorias, MAX_CATEGORIAS);
    total = carregarProdutos(produtos, MAX_PRODUTOS);

    do {
        exibirMenu();
        opcao = lerOpcaoMenu();

        switch (opcao) {
            case OPC_CADASTRAR:
                total = cadastrarProduto(produtos, total,
                                         categorias, totalCategorias);
                salvarProdutos(produtos, total);
                break;
            case OPC_LISTAR:
                listarProdutos(produtos, total, categorias, totalCategorias);
                break;
            case OPC_PESQUISAR:
                pesquisarProduto(produtos, total, categorias, totalCategorias);
                break;
            case OPC_CATEGORIAS:
                totalCategorias = menuCategorias(categorias, totalCategorias);
                salvarCategorias(categorias, totalCategorias);
                break;
            case OPC_SAIR:
                salvarCategorias(categorias, totalCategorias);
                salvarProdutos(produtos, total);
                printf("\nDados salvos. Saindo do sistema...\n");
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
                break;
        }
    } while (opcao != OPC_SAIR);

    return EXIT_SUCCESS;
}

void exibirMenu(void) {
    printf("\n===== MENU PRINCIPAL =====\n");
    printf("%d - Cadastrar Produto\n", OPC_CADASTRAR);
    printf("%d - Listar Produtos\n", OPC_LISTAR);
    printf("%d - Pesquisar Produto\n", OPC_PESQUISAR);
    printf("%d - Categorias\n", OPC_CATEGORIAS);
    printf("%d - Sair\n", OPC_SAIR);
    printf("===========================\n");
}
