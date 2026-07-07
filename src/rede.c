#include "include/rede.h"

// Começa com uma rede vazia
void inicializar_rede(Rede *r) {
    r->num_membros = 0;
    for (int i = 0; i < MAX_MEMBROS; i++) {
        r->adj[i] = NULL;
    }
}

// Inscreve uma nova pessoa na rede e devole o ID dela
int adicionar_membro(Rede *r, const char *nome, const char *email) {
    if (r->num_membros >= MAX_MEMBROS) {
        printf("[ERRO] Rede cheia.\n");
        return -1;
    }
    int id = r->num_membros;
    Membro *m = &r->membros[id];
    m->id = id;
    strncpy(m->nome,  nome,  MAX_NOME  - 1);
    strncpy(m->email, email, MAX_EMAIL - 1);
    m->nome[MAX_NOME - 1]   = '\0';
    m->email[MAX_EMAIL - 1] = '\0';

    inicializar_fila(&m->inbox,    LIMITE_INBOX);
    inicializar_fila(&m->spam_box, LIMITE_SPAM);

    r->adj[id] = NULL;
    r->num_membros++;
    printf("[OK] Membro '%s' adicionado com id=%d.\n", nome, id);
    return id;
}

// Conecta dois membros (amizade em ambas as direções)
int adicionar_ligacao(Rede *r, int id1, int id2) {
    if (id1 < 0 || id1 >= r->num_membros ||
        id2 < 0 || id2 >= r->num_membros) {
        printf("[ERRO] Id inválido.\n");
        return 0;
    }
    if (id1 == id2) {
        printf("[ERRO] Um membro não se pode ligar a si próprio.\n");
        return 0;
    }
    if (existe_ligacao(r, id1, id2)) {
        printf("[AVISO] Ligação já existe.\n");
        return 0;
    }

    // Adiciona id2 como amigo de id1
    NoAdj *n1 = (NoAdj *)malloc(sizeof(NoAdj));
    n1->id_membro = id2;
    n1->prox      = r->adj[id1];
    r->adj[id1]   = n1;

    // Adiciona id1 como amigo de id2 (bidirecional)
    NoAdj *n2 = (NoAdj *)malloc(sizeof(NoAdj));
    n2->id_membro = id1;
    n2->prox      = r->adj[id2];
    r->adj[id2]   = n2;

    printf("[OK] Ligação criada entre '%s' e '%s'.\n",
           r->membros[id1].nome, r->membros[id2].nome);
    return 1;
}

int existe_ligacao(Rede *r, int id1, int id2) {
    NoAdj *p = r->adj[id1];
    while (p) {
        if (p->id_membro == id2) return 1;
        p = p->prox;
    }
    return 0;
}

void listar_membros(const Rede *r) {
    printf("\n=== MEMBROS DA REDE (%d) ===\n", r->num_membros);
    for (int i = 0; i < r->num_membros; i++) {
        printf("  [%d] %s <%s>\n", i, r->membros[i].nome, r->membros[i].email);
    }
}

void listar_ligacoes(const Rede *r) {
    printf("\n=== LIGACOES DA REDE ===\n");
    for (int i = 0; i < r->num_membros; i++) {
        printf("  %s -> ", r->membros[i].nome);
        NoAdj *p = r->adj[i];
        if (!p) { printf("(nenhuma)\n"); continue; }
        while (p) {
            printf("%s", r->membros[p->id_membro].nome);
            if (p->prox) printf(", ");
            p = p->prox;
        }
        printf("\n");
    }
}

int encontrar_membro_por_nome(const Rede *r, const char *nome) {
    for (int i = 0; i < r->num_membros; i++) {
        if (strcmp(r->membros[i].nome, nome) == 0) return i;
    }
    return -1;
}
