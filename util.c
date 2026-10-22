#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <limits.h>

#include "util.h"

#define TAM_LINHA 64

/* ===================== LEITURA DE DADOS ===================== */

void limparBufferEntrada(void) {
    int caractere;

    while ((caractere = getchar()) != '\n' && caractere != EOF) {
        /* Descarta o restante da linha */
    }
}

/* Le uma linha inteira, remove o '\n' e descarta o que passar do tamanho. */
void lerLinha(char destino[], size_t tamanho) {
    size_t tam;

    if (fgets(destino, (int)tamanho, stdin) == NULL) {
        printf("\nEntrada encerrada. Finalizando o programa.\n");
        exit(EXIT_FAILURE);
    }

    tam = strlen(destino);
    if (tam > 0 && destino[tam - 1] == '\n') {
        destino[tam - 1] = '\0';
    } else {
        limparBufferEntrada();
    }
}

int lerInteiro(const char mensagem[]) {
    char linha[TAM_LINHA];
    char *fim;
    long valor;

    for (;;) {
        printf("%s", mensagem);
        lerLinha(linha, sizeof(linha));

        errno = 0;
        valor = strtol(linha, &fim, 10);

        if (fim == linha || !soTemEspacos(fim)) {
            printf("Entrada invalida! Digite um numero inteiro.\n");
        } else if (errno == ERANGE || valor > INT_MAX || valor < INT_MIN) {
            printf("Numero fora do limite permitido.\n");
        } else {
            return (int)valor;
        }
    }
}

float lerFloat(const char mensagem[]) {
    char linha[TAM_LINHA];
    char *fim;
    char *virgula;
    double valor;

    for (;;) {
        printf("%s", mensagem);
        lerLinha(linha, sizeof(linha));

        virgula = strchr(linha, ',');
        if (virgula != NULL) {
            *virgula = '.';
        }

        errno = 0;
        valor = strtod(linha, &fim);

        if (fim == linha || !soTemEspacos(fim)) {
            printf("Entrada invalida! Digite um numero (ex.: 12.50).\n");
        } else if (errno == ERANGE || valor != valor ||
                   valor > FLT_MAX || valor < -FLT_MAX) {
            printf("Numero fora do limite permitido.\n");
        } else {
            return (float)valor;
        }
    }
}

/* ===================== VALIDACOES GENERICAS ===================== */

int soTemEspacos(const char *texto) {
    while (*texto != '\0') {
        if (!isspace((unsigned char)*texto)) {
            return 0;
        }
        texto++;
    }
    return 1;
}

int textoPreenchido(const char texto[]) {
    return !soTemEspacos(texto);
}

int numeroPositivo(double valor) {
    return valor > 0;
}

int numeroNaoNegativo(double valor) {
    return valor >= 0;
}

int letraNoIntervalo(char letra, char minimo, char maximo) {
    return letra >= minimo && letra <= maximo;
}

int extrairLetraUnica(const char linha[], char *letra) {
    const char *p = linha;

    while (isspace((unsigned char)*p)) {
        p++;
    }

    if (*p != '\0' && soTemEspacos(p + 1)) {
        *letra = (char)toupper((unsigned char)*p);
        return 1;
    }
    return 0;
}
