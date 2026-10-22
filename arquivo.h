#ifndef ARQUIVO_H
#define ARQUIVO_H

#include "categoria.h"
#include "produto.h"

#define ARQUIVO_CATEGORIAS "categorias.txt"
#define ARQUIVO_PRODUTOS "produtos.txt"

/* Carregam os dados do arquivo e devolvem quantos registros foram lidos.
   Se o arquivo nao existir, devolvem 0 (lista vazia). */
int carregarCategorias(Categoria lista[], int maximo);
int carregarProdutos(Produto lista[], int maximo);

/* Gravam a lista inteira no arquivo. Devolvem 1 se deu certo, 0 se falhou. */
int salvarCategorias(const Categoria lista[], int total);
int salvarProdutos(const Produto lista[], int total);

#endif
