#include "include/rede.h"

// Cria lista de palavras spam vazia
void inicializar_lista_spam(ListaPalavrasSpam *ls, int k) {
    ls->inicio = NULL;
    ls->total  = 0;
    ls->k      = k;
}

// Adiciona uma palavra que caracteriza spam
void adicionar_palavra_spam(ListaPalavrasSpam *ls, const char *palavra) {
    // Primeiro vê se a palavra já tá adicionada
    NoPalavra *p = ls->inicio;
    while (p) {
        if (strcmp(p->palavra, palavra) == 0) {
            printf("[AVISO] Palavra '%s' já está no universo spam.\n", palavra);
            return;
        }
        p = p->prox;
    }
    NoPalavra *novo = (NoPalavra *)malloc(sizeof(NoPalavra));
    if (!novo) { perror("malloc"); return; }
    strncpy(novo->palavra, palavra, 49);
    novo->palavra[49] = '\0';
    novo->prox  = ls->inicio;
    ls->inicio  = novo;
    ls->total++;
    printf("[OK] Palavra '%s' adicionada ao universo spam (P=%d, k=%d).\n",
           palavra, ls->total, ls->k);
}

// Verifica se a mensagem tem spam (precisa ter pelo menos k palavras spam)
int e_spam(const ListaPalavrasSpam *ls, const char *mensagem) {
    if (ls->total == 0 || ls->k <= 0) return 0;

    // Transforma tudo pra minúscula pra comparação funcionar independente de maiúscula/minúscula
    char copia[MAX_MENSAGEM];
    str_tolower_copy(copia, mensagem, MAX_MENSAGEM);

    int contagem = 0;
    NoPalavra *p = ls->inicio;
    while (p) {
        char palavra_lower[50];
        str_tolower_copy(palavra_lower, p->palavra, 50);
        if (strstr(copia, palavra_lower) != NULL) {
            contagem++;
        }
        p = p->prox;
    }
    return (contagem >= ls->k) ? 1 : 0;
}

// Mostra todas as palavras que caracterizam spam
void listar_palavras_spam(const ListaPalavrasSpam *ls) {
    printf("\n=== UNIVERSO SPAM (P=%d, k=%d) ===\n", ls->total, ls->k);
    NoPalavra *p = ls->inicio;
    int i = 1;
    while (p) {
        printf("  %d. %s\n", i++, p->palavra);
        p = p->prox;
    }
    if (ls->total == 0) printf("  (vazio)\n");
}

// Libera a memória da lista de spam
void libertar_lista_spam(ListaPalavrasSpam *ls) {
    NoPalavra *p = ls->inicio;
    while (p) {
        NoPalavra *tmp = p->prox;
        free(p);
        p = tmp;
    }
    ls->inicio = NULL;
    ls->total  = 0;
}
