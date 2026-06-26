#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TEXTO 500
#define MAX_PALAVRA 30


// Palavra usada no dicionário para ajudar a identificar spam.
typedef struct {
    char palavra[MAX_PALAVRA];
} PalavraSpam;

// Estrutura de uma mensagem de e-mail.
typedef struct {
    char remetente[50];
    char texto[MAX_TEXTO];
} Mensagem;

// Estrutura de cada nó de uma fila encadeada de mensagens.
typedef struct NoFila {
    Mensagem msg;
    struct NoFila* proximo;
} NoFila;

//Estrutura de uma caixa de correio, com fila, limite e quantidade atual de mensagens.
typedef struct {
    NoFila* frente;
    NoFila* tras;
    int total_atual;
    int limite_maximo;
} CaixaCorreio;

// Estrutura para cada elemento da lista.
typedef struct Vizinho {
    int id_vizinho;
    struct Vizinho* proximo;
} Vizinho;

// Estrutura de um usuário da rede.
typedef struct {
    int id;
    char nome[50];
    Vizinho* lista_amigos;
    CaixaCorreio caixa_normal;
    CaixaCorreio caixa_spam;
} Usuario;

// Estrutura principal do sistema.
typedef struct {
    Usuario* usuarios;
    int total_usuarios;
} RedeComputadores;


// Funções para criar e gerenciar a rede e as conexões entre usuários.
RedeComputadores* criar_rede(int capacidade);
void cadastrar_usuario(RedeComputadores* rede, int id, char* nome, int limite_caixa);
void conectar_amigos(RedeComputadores* rede, int id1, int id2);
int sao_vizinhos_diretos(RedeComputadores* rede, int id1, int id2);

// Funções para trabalhar com as caixas de correio e suas filas.
void iniciar_caixa(CaixaCorreio* caixa, int limite);
int caixa_esta_cheia(CaixaCorreio* caixa);
int enfileirar_mensagem(CaixaCorreio* caixa, Mensagem msg);
Mensagem desenfileirar_mensagem(CaixaCorreio* caixa);

// Função para verificar se uma mensagem deve ser marcada como spam.
int verificar_se_e_spam(char* texto_mensagem, PalavraSpam* dicionario, int total_p, int k);

// Funções para envio e visualização de mensagens entre usuários.
void enviar_email(RedeComputadores* rede, int id_remetente, int id_destinatario, char* texto, PalavraSpam* dicionario, int total_p, int k);
void exibir_caixas_usuario(RedeComputadores* rede, int id_usuario);

// Função para liberar a memória alocada pela rede.
void destruir_rede(RedeComputadores* rede);