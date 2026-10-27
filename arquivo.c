#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "arquivo.h"
#include "util.h"

#define TAM_LINHA_ARQUIVO 256

/* Remove \n e \r do final da linha. */
static void removerQuebraDeLinha(char linha[]) {
    size_t tam = strlen(linha);

    while (tam > 0 && (linha[tam - 1] == '\n' || linha[tam - 1] == '\r')) {
        linha[tam - 1] = '\0';
        tam--;
    }
}

/* ===================== CATEGORIAS ===================== */
/* Formato de cada linha:  codigo;nome */

int carregarCategorias(Categoria lista[], int maximo) {
    FILE *arq;
    char linha[TAM_LINHA_ARQUIVO];
    char *fim;
    long codigo;
    int total = 0;

    arq = fopen(ARQUIVO_CATEGORIAS, "r");
    if (arq == NULL) {
        return 0; /* primeira execucao: comeca vazio */
    }

    while (total < maximo && fgets(linha, sizeof(linha), arq) != NULL) {
        removerQuebraDeLinha(linha);

        codigo = strtol(linha, &fim, 10);
        if (fim == linha || *fim != ';') {
            continue; /* linha invalida */
        }
        fim++; /* pula o ';' */

        if (codigo <= 0 || !textoPreenchido(fim)) {
            continue;
        }
        if (buscarCategoriaPorCodigo(lista, total, (int)codigo) != -1) {
            continue; /* codigo repetido */
        }

        lista[total].codigo = (int)codigo;
        strncpy(lista[total].nome, fim, TAM_NOME_CATEGORIA - 1);
        lista[total].nome[TAM_NOME_CATEGORIA - 1] = '\0';
        total++;
    }

    fclose(arq);
    return total;
}

int salvarCategorias(const Categoria lista[], int total) {
    FILE *arq;
    int i;

    arq = fopen(ARQUIVO_CATEGORIAS, "w");
    if (arq == NULL) {
        printf("\nErro: nao foi possivel salvar %s.\n", ARQUIVO_CATEGORIAS);
        return 0;
    }

    for (i = 0; i < total; i++) {
        fprintf(arq, "%d;%s\n", lista[i].codigo, lista[i].nome);
    }

    fclose(arq);
    return 1;
}

/* ===================== PRODUTOS ===================== */
/* Formato de cada linha:
   codigo;prateleira;preco;quantidade;codigoCategoria;ativo;nome
   (o nome fica por ultimo para poder conter qualquer caractere)
   Arquivos antigos, sem o campo "ativo", tambem sao aceitos (ativo = 1). */

int carregarProdutos(Produto lista[], int maximo) {
    FILE *arq;
    char linha[TAM_LINHA_ARQUIVO];
    Produto p;
    int lidos;
    int consumidos;
    int total = 0;

    arq = fopen(ARQUIVO_PRODUTOS, "r");
    if (arq == NULL) {
        return 0; /* primeira execucao: comeca vazio */
    }

    while (total < maximo && fgets(linha, sizeof(linha), arq) != NULL) {
        removerQuebraDeLinha(linha);

        consumidos = 0;
        p.ativo = 1;
        lidos = sscanf(linha, "%d;%c;%f;%d;%d;%d;%n", &p.codigo,
                       &p.prateleira, &p.preco, &p.quantidade,
                       &p.codigoCategoria, &p.ativo, &consumidos);

        if (lidos != 6 || consumidos == 0 ||
            (p.ativo != 0 && p.ativo != 1)) {
            /* Tenta o formato antigo (sem o campo "ativo"). */
            consumidos = 0;
            p.ativo = 1;
            lidos = sscanf(linha, "%d;%c;%f;%d;%d;%n", &p.codigo,
                           &p.prateleira, &p.preco, &p.quantidade,
                           &p.codigoCategoria, &consumidos);

            if (lidos != 5 || consumidos == 0) {
                continue; /* linha invalida */
            }
        }
        if (!numeroPositivo(p.codigo) || !numeroPositivo(p.preco) ||
            !numeroNaoNegativo(p.quantidade) ||
            !numeroPositivo(p.codigoCategoria) ||
            !letraNoIntervalo(p.prateleira, PRATELEIRA_MIN, PRATELEIRA_MAX) ||
            !textoPreenchido(linha + consumidos)) {
            continue;
        }
        if (buscarPorCodigo(lista, total, p.codigo) != NAO_ENCONTRADO) {
            continue; /* codigo repetido */
        }

        strncpy(p.nome, linha + consumidos, TAM_NOME - 1);
        p.nome[TAM_NOME - 1] = '\0';
        p.disponivel = calcularDisponibilidade(p.quantidade);

        lista[total] = p;
        total++;
    }

    fclose(arq);
    return total;
}

int salvarProdutos(const Produto lista[], int total) {
    FILE *arq;
    int i;

    arq = fopen(ARQUIVO_PRODUTOS, "w");
    if (arq == NULL) {
        printf("\nErro: nao foi possivel salvar %s.\n", ARQUIVO_PRODUTOS);
        return 0;
    }

    for (i = 0; i < total; i++) {
        fprintf(arq, "%d;%c;%.6f;%d;%d;%d;%s\n", lista[i].codigo,
                lista[i].prateleira, lista[i].preco, lista[i].quantidade,
                lista[i].codigoCategoria, lista[i].ativo, lista[i].nome);
    }

    fclose(arq);
    return 1;
}
