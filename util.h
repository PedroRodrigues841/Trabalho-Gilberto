#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>

/* ===== Leitura de dados (genericas) ===== */
void limparBufferEntrada(void);
void lerLinha(char destino[], size_t tamanho);
int lerInteiro(const char mensagem[]);
float lerFloat(const char mensagem[]);

/* ===== Validacoes genericas ===== */
int soTemEspacos(const char *texto);
int textoPreenchido(const char texto[]);
int numeroPositivo(double valor);        /* valor > 0  */
int numeroNaoNegativo(double valor);     /* valor >= 0 */
int letraNoIntervalo(char letra, char minimo, char maximo);

/* Se a linha tem exatamente um caractere (ignorando espacos),
   guarda-o em maiusculo em *letra e devolve 1. Senao devolve 0. */
int extrairLetraUnica(const char linha[], char *letra);

#endif
