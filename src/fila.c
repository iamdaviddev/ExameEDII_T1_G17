#include "include/rede.h"

// Cria uma fila vazia pra guardar mensagens
void inicializar_fila(FilaMensagens *f, int limite) {
    f->inicio = NULL;
    f->fim    = NULL;
    f->total  = 0;
    f->limite = limite;
}

// Bota uma mensagem na fila. Retorna 1 se deu certo, 0 se tá cheio
int enfileirar(FilaMensagens *f, const char *remetente,
               const char *conteudo, int is_spam) {
    if (f->total >= f->limite) {
        return 0;   // Caixa tá cheia, a mensagem é descartada
    }
    Mensagem *nova = (Mensagem *)malloc(sizeof(Mensagem));
    if (!nova) { perror("malloc"); return 0; }

    strncpy(nova->remetente, remetente, MAX_NOME - 1);
    nova->remetente[MAX_NOME - 1] = '\0';
    strncpy(nova->conteudo, conteudo, MAX_MENSAGEM - 1);
    nova->conteudo[MAX_MENSAGEM - 1] = '\0';
    nova->is_spam = is_spam;
    nova->ant     = f->fim;
    nova->prox    = NULL;

    if (f->fim) {
        f->fim->prox = nova;
    } else {
        f->inicio = nova;
    }
    f->fim = nova;
    f->total++;
    return 1;
}

// Tira e devolve a primeira mensagem da fila (ou NULL se tá vazia)
Mensagem *desenfileirar(FilaMensagens *f) {
    if (!f->inicio) return NULL;

    Mensagem *msg = f->inicio;
    f->inicio = msg->prox;
    if (f->inicio) {
        f->inicio->ant = NULL;
    } else {
        f->fim = NULL;
    }
    msg->prox = NULL;
    msg->ant  = NULL;
    f->total--;
    return msg;
}

// Mostra todas as mensagens que tem na fila
void listar_fila(const FilaMensagens *f, const char *titulo) {
    printf("\n  -- %s (%d/%d) --\n", titulo, f->total, f->limite);
    if (f->total == 0) {
        printf("     (vazia)\n");
        return;
    }
    Mensagem *p = f->inicio;
    int i = 1;
    while (p) {
        printf("  %d. De: %-15s | %s\n", i++, p->remetente, p->conteudo);
        p = p->prox;
    }
}

// Limpa a memória da fila quando termina
void libertar_fila(FilaMensagens *f) {
    Mensagem *p = f->inicio;
    while (p) {
        Mensagem *tmp = p->prox;
        free(p);
        p = tmp;
    }
    f->inicio = NULL;
    f->fim    = NULL;
    f->total  = 0;
}
