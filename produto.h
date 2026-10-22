#ifndef PRODUTO_H
#define PRODUTO_H

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
} Produto;

/* Le a opcao escolhida no menu principal. */
int lerOpcaoMenu(void);

/* Regras do produto */
int calcularDisponibilidade(int quantidade);
int buscarPorCodigo(const Produto lista[], int total, int codigo);
int buscarPorNome(const Produto lista[], int total, const char nome[]);

/* Operacoes e telas */
int cadastrarProduto(Produto lista[], int total);
void listarProdutos(const Produto lista[], int total);
void exibirProduto(Produto produto);
void pesquisarProduto(const Produto lista[], int total);

#endif
