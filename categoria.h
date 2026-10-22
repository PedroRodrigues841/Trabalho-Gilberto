#ifndef CATEGORIA_H
#define CATEGORIA_H

#define TAM_NOME_CATEGORIA 30
#define MAX_CATEGORIAS 20

typedef struct {
    int codigo;
    char nome[TAM_NOME_CATEGORIA];
} Categoria;

/* Devolve a posicao da categoria na lista ou -1 se nao existir. */
int buscarCategoriaPorCodigo(const Categoria lista[], int total, int codigo);

/* Devolve o nome da categoria ou um texto padrao se o codigo nao existir. */
const char *nomeDaCategoria(const Categoria lista[], int total, int codigo);

/* Cadastra uma categoria e devolve o novo total. */
int cadastrarCategoria(Categoria lista[], int total);

void listarCategorias(const Categoria lista[], int total);

/* Submenu de categorias. Devolve o total de categorias ao voltar. */
int menuCategorias(Categoria lista[], int total);

#endif
