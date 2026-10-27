#ifndef PRODUTO_H
#define PRODUTO_H

#include "categoria.h"

#define TAM_NOME 50
#define MAX_PRODUTOS 50
#define NAO_ENCONTRADO -1
#define PRATELEIRA_MIN 'A'
#define PRATELEIRA_MAX 'D'

typedef struct {
    int codigo;
    char nome[TAM_NOME];
    char prateleira;
    float preco;
    int quantidade;
    int disponivel;
    int codigoCategoria;
    int ativo; /* 1 = normal, 0 = removido (exclusao logica) */
} Produto;

/* Le a opcao escolhida no menu principal. */
int lerOpcaoMenu(void);

/* Regras do produto */
int calcularDisponibilidade(int quantidade);
/* Procura entre TODOS os produtos, inclusive os removidos (usada para
   impedir codigo repetido). */
int buscarPorCodigo(const Produto lista[], int total, int codigo);
/* Procuram apenas entre os produtos ativos. */
int buscarAtivoPorCodigo(const Produto lista[], int total, int codigo);
int buscarPorNome(const Produto lista[], int total, const char nome[]);

/* Operacoes e telas (recebem as categorias para mostrar o nome) */
int cadastrarProduto(Produto lista[], int total,
                     const Categoria categorias[], int totalCategorias);
void listarProdutos(const Produto lista[], int total,
                    const Categoria categorias[], int totalCategorias);
void exibirProduto(Produto produto,
                   const Categoria categorias[], int totalCategorias);
void pesquisarProduto(const Produto lista[], int total,
                      const Categoria categorias[], int totalCategorias);

/* Entrada ou saida de itens no estoque de um produto ativo. */
void alterarEstoque(Produto lista[], int total);

/* Exclusao logica: marca o produto como inativo (ativo = 0). */
void removerProduto(Produto lista[], int total);

#endif
