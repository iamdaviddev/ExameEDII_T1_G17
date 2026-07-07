#ifndef REDE_H
#define REDE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define MAX_NOME       50
#define MAX_EMAIL      100
#define MAX_MENSAGEM   500
#define MAX_MEMBROS    100
#define LIMITE_INBOX   10
#define LIMITE_SPAM    5


// Nó que guarda uma palavra spam individual
typedef struct NoPalavra {
    char palavra[50];
    struct NoPalavra *prox;
} NoPalavra;

// Universo de palavras spam com limite k (precisa de k palavras pra ser spam)
typedef struct {
    NoPalavra *inicio;
    int total;
    int k;
} ListaPalavrasSpam;

// Estrutura de uma mensagem dentro da fila
typedef struct Mensagem {
    char remetente[MAX_NOME];
    char conteudo[MAX_MENSAGEM];
    int  is_spam;
    struct Mensagem *ant;
    struct Mensagem *prox;
} Mensagem;

// Fila pra guardar as mensagens (entra de um lado, sai do outro)
typedef struct {
    Mensagem *inicio;
    Mensagem *fim;
    int       total;
    int       limite;
} FilaMensagens;

/* Membro da rede */
typedef struct Membro {
    int  id;
    char nome[MAX_NOME];
    char email[MAX_EMAIL];
    FilaMensagens inbox;
    FilaMensagens spam_box;
} Membro;

// Cada pessoa tem uma lista de amigos (nó do grafo)
typedef struct NoAdj {
    int          id_membro;
    struct NoAdj *prox;
} NoAdj;

typedef struct {
    Membro  membros[MAX_MEMBROS];
    NoAdj  *adj[MAX_MEMBROS];
    int     num_membros;
} Rede;


/* Rede */
void inicializar_rede(Rede *r);
int  adicionar_membro(Rede *r, const char *nome, const char *email);
int  adicionar_ligacao(Rede *r, int id1, int id2);
int  existe_ligacao(Rede *r, int id1, int id2);
void listar_membros(const Rede *r);
void listar_ligacoes(const Rede *r);
int  encontrar_membro_por_nome(const Rede *r, const char *nome);

/* Spam */
void inicializar_lista_spam(ListaPalavrasSpam *ls, int k);
void adicionar_palavra_spam(ListaPalavrasSpam *ls, const char *palavra);
int  e_spam(const ListaPalavrasSpam *ls, const char *mensagem);
void listar_palavras_spam(const ListaPalavrasSpam *ls);
void libertar_lista_spam(ListaPalavrasSpam *ls);

/* Fila de mensagens */
void inicializar_fila(FilaMensagens *f, int limite);
int  enfileirar(FilaMensagens *f, const char *remetente, const char *conteudo, int is_spam);
Mensagem *desenfileirar(FilaMensagens *f);
void listar_fila(const FilaMensagens *f, const char *titulo);
void libertar_fila(FilaMensagens *f);

/* Envio */
int enviar_mensagem(Rede *r, ListaPalavrasSpam *ls, int id_orig, int id_dest, const char *conteudo);
void ver_caixa(const Rede *r, int id);

/* Utilitários */
void str_tolower_copy(char *dest, const char *src, int max);
void limpar_buffer(void);
void menu_principal(Rede *r, ListaPalavrasSpam *ls);

#endif /* REDE_H */
