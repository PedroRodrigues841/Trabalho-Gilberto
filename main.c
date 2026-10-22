#include <stdio.h>
#include <stdlib.h>

#include "produto.h"

enum { OPC_CADASTRAR = 1, OPC_LISTAR, OPC_PESQUISAR, OPC_SAIR };

void exibirMenu(void);

int main(void) {
    Produto produtos[MAX_PRODUTOS];
    int total = 0;
    int opcao;

    do {
        exibirMenu();
        opcao = lerOpcaoMenu();

        switch (opcao) {
            case OPC_CADASTRAR:
                total = cadastrarProduto(produtos, total);
                break;
            case OPC_LISTAR:
                listarProdutos(produtos, total);
                break;
            case OPC_PESQUISAR:
                pesquisarProduto(produtos, total);
                break;
            case OPC_SAIR:
                printf("\nSaindo do sistema...\n");
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
    printf("%d - Sair\n", OPC_SAIR);
    printf("===========================\n");
}
