#ifndef SIGTAG_H
#define SIGTAG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* DEFINIÇÕES GERAIS */
#define MAX_NOME       100
#define MAX_LOCAL      100
#define MAX_MINERAL    50
#define MAX_NOS_GRAFO  20
#define MAX_AMOSTRAS   200
#define NUM_OPCOES     13

/* Struct Amostra Geológica */
typedef struct {
    int    codigo;
    char   nome[MAX_NOME];
    char   local_extracao[MAX_LOCAL];
    char   mineral[MAX_MINERAL];
    float  teor;
    int    prioridade;
    char   data[12];
} Amostra;

/* Struct BST */
typedef struct NoBST {
    Amostra        amostra;
    struct NoBST  *esq;
    struct NoBST  *dir;
} NoBST;

/* Struct AVL */
typedef struct NoAVL {
    Amostra        amostra;
    struct NoAVL  *esq;
    struct NoAVL  *dir;
    int            altura;
} NoAVL;

/* GRAFO — REDE LOGÍSTICA */
typedef enum { EXTRACAO, LABORATORIO, EXPORTACAO } TipoPonto;

typedef struct {
    int       id;
    char      nome[MAX_NOME];
    TipoPonto tipo;
} Ponto;

typedef struct {
    int origem;
    int destino;
    int peso;
} Aresta;

typedef struct {
    Ponto  pontos[MAX_NOS_GRAFO];
    int    adj[MAX_NOS_GRAFO][MAX_NOS_GRAFO];
    int    n_pontos;
    int    n_arestas;
} Grafo;

/* Sistema de Menu Adaptativo */
typedef struct {
    int  id;
    char descricao[MAX_NOME];
    int  usos;
} OpcaoMenu;

typedef struct {
    OpcaoMenu opcoes[NUM_OPCOES];
} MenuAdaptativo;

/* Struct do Sistema */
typedef struct {
    NoBST        *root_bst;
    NoAVL        *root_avl;
    Grafo         grafo;
    MenuAdaptativo menu;
    Amostra       lista[MAX_AMOSTRAS];
    int           total_amostras;
    long long     comp_bst;
    long long     comp_avl;
    long long     comp_bubble;
    long long     comp_selection;
    long long     trocas_bubble;
    long long     trocas_selection;
} Sistema;

/* BST */
NoBST* bst_inserir(NoBST *raiz, Amostra a, long long *comp);
NoBST* bst_buscar(NoBST *raiz, int codigo, long long *comp);
NoBST* bst_remover(NoBST *raiz, int codigo, long long *comp);
void   bst_emordem(NoBST *raiz, Amostra *out, int *idx);
void   bst_destruir(NoBST *raiz);

/* AVL */
NoAVL* avl_inserir(NoAVL *raiz, Amostra a, long long *comp);
NoAVL* avl_buscar(NoAVL *raiz, int codigo, long long *comp);
NoAVL* avl_remover(NoAVL *raiz, int codigo, long long *comp);
void   avl_emordem(NoAVL *raiz, Amostra *out, int *idx);
void   avl_destruir(NoAVL *raiz);
int    avl_altura(NoAVL *no);

/* ORDENAÇÃO */
void bubble_sort_teor(Amostra *arr, int n, long long *comp, long long *trocas);
void selection_sort_prioridade(Amostra *arr, int n, long long *comp, long long *trocas);

/* GRAFO */
void giniciar(Grafo *g);
int  gadd_ponto(Grafo *g, const char *nome, TipoPonto tipo);
void gadd_aresta(Grafo *g, int orig, int dest, int peso);
void gdijkstra(Grafo *g, int origem, int destino, int *caminho, int *tam_caminho, int *distancia);
void gimprimir(Grafo *g);

/* MENU */
void menu_iniciar(MenuAdaptativo *m);
void menu_registar_uso(MenuAdaptativo *m, int id);
void menu_ordenar(MenuAdaptativo *m);
void menu_imprimir(MenuAdaptativo *m);

/* SISTEMA */
void iniciar(Sistema *s);
void popular_demo(Sistema *s);
void executar(Sistema *s);
void inserir_amostra(Sistema *s);
void buscar_amostra(Sistema *s);
void remover_amostra(Sistema *s);
void listar_inorder(Sistema *s);
void ordenar_bubble(Sistema *s);
void ordenar_selection(Sistema *s);
void comparar_arvores(Sistema *s);
void comparar_ordenacao(Sistema *s);
void add_ponto_grafo(Sistema *s);
void calcular_rota(Sistema *s);
void sugerir_estrutura(Sistema *s);
void ver_estatisticas(Sistema *s);

/* FICHEIROS */
int  ler_amostras(const char *caminho, Amostra *arr, int max);
void ler_grafo(const char *caminho, Grafo *g);
void escrever_relatorio(const char *caminho, Sistema *s);

/* UTILITÁRIOS */
void limpar_tela(void);
void pausar(void);
void imprimir_separador(const char *titulo);
void imprimir_amostra(const Amostra *a);
int  ler_inteiro(const char *prompt, int min, int max);
float ler_float(const char *prompt, float min, float max);
void ler_string(const char *prompt, char *dest, int tam);

#endif /* SIGTAG_H */
