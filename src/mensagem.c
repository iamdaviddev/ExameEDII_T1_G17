#include "include/rede.h"

// Envia uma mensagem de uma pessoa pra outra (só funciona se são amigos diretos)
int enviar_mensagem(Rede *r, ListaPalavrasSpam *ls,
                   int id_orig, int id_dest, const char *conteudo) {

    if (id_orig < 0 || id_orig >= r->num_membros ||
        id_dest < 0 || id_dest >= r->num_membros) {
        printf("[ERRO] Id de membro inválido.\n");
        return 0;
    }
    if (id_orig == id_dest) {
        printf("[ERRO] Não é possível enviar mensagem para si próprio.\n");
        return 0;
    }
    if (!existe_ligacao(r, id_orig, id_dest)) {
        printf("[ERRO] Não existe ligação directa entre '%s' e '%s'.\n",
               r->membros[id_orig].nome, r->membros[id_dest].nome);
        return 0;
    }

    Membro *dest    = &r->membros[id_dest];
    const char *rem = r->membros[id_orig].nome;
    int spam        = e_spam(ls, conteudo);

    if (spam) {
        // Se é spam, tenta botar na pasta de spam
        if (dest->spam_box.total >= dest->spam_box.limite) {
            printf("[INFO] Caixa SPAM de '%s' cheia — mensagem descartada.\n",
                   dest->nome);
            return 0;
        }
        enfileirar(&dest->spam_box, rem, conteudo, 1);
        printf("[SPAM] Mensagem de '%s' para '%s' classificada como SPAM e "
               "encaminhada para spam_box.\n", rem, dest->nome);
    } else {
        // Se não é spam, bota na inbox normal
        if (dest->inbox.total >= dest->inbox.limite) {
            printf("[INFO] Inbox de '%s' cheia — mensagem descartada.\n",
                   dest->nome);
            return 0;
        }
        enfileirar(&dest->inbox, rem, conteudo, 0);
        printf("[OK] Mensagem de '%s' entregue na inbox de '%s'.\n",
               rem, dest->nome);
    }
    return 1;
}

// Mostra as mensagens de um membro
void ver_caixa(const Rede *r, int id) {
    if (id < 0 || id >= r->num_membros) {
        printf("[ERRO] Id inválido.\n");
        return;
    }
    const Membro *m = &r->membros[id];
    printf("\n========== CAIXA DE: %s <%s> ==========\n",
           m->nome, m->email);
    listar_fila(&m->inbox,    "INBOX");
    listar_fila(&m->spam_box, "SPAM BOX");
    printf("==========================================\n");
}
