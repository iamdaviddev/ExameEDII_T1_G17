#include "sigtag.h"

static NoBST* novo_no_bst(Amostra a) {
    NoBST *no = (NoBST*)malloc(sizeof(NoBST));
    if (!no) {
        fprintf(stderr, "Erro: sem memória (BST)\n");
        exit(1);
    }
    no->amostra = a;
    no->esq = no->dir = NULL;
    return no;
}

NoBST* bst_inserir(NoBST *raiz, Amostra a, long long *comp) {
    if (!raiz) return novo_no_bst(a);
    (*comp)++;
    if (a.codigo < raiz->amostra.codigo)
        raiz->esq = bst_inserir(raiz->esq, a, comp);
    else if (a.codigo > raiz->amostra.codigo)
        raiz->dir = bst_inserir(raiz->dir, a, comp);
    return raiz;
}

NoBST* bst_buscar(NoBST *raiz, int codigo, long long *comp) {
    if (!raiz) return NULL;
    (*comp)++;
    if (codigo == raiz->amostra.codigo) return raiz;
    if (codigo < raiz->amostra.codigo)
        return bst_buscar(raiz->esq, codigo, comp);
    return bst_buscar(raiz->dir, codigo, comp);
}

static NoBST* bst_min(NoBST *no) {
    while (no->esq) no = no->esq;
    return no;
}

NoBST* bst_remover(NoBST *raiz, int codigo, long long *comp) {
    if (!raiz) return NULL;
    (*comp)++;
    if (codigo < raiz->amostra.codigo)
        raiz->esq = bst_remover(raiz->esq, codigo, comp);
    else if (codigo > raiz->amostra.codigo)
        raiz->dir = bst_remover(raiz->dir, codigo, comp);
    else {
        if (!raiz->esq) {
            NoBST *tmp = raiz->dir;
            free(raiz);
            return tmp;
        } else if (!raiz->dir) {
            NoBST *tmp = raiz->esq;
            free(raiz);
            return tmp;
        }
        NoBST *suc = bst_min(raiz->dir);
        raiz->amostra = suc->amostra;
        raiz->dir = bst_remover(raiz->dir, suc->amostra.codigo, comp);
    }
    return raiz;
}

void bst_emordem(NoBST *raiz, Amostra *out, int *idx) {
    if (!raiz) return;
    bst_emordem(raiz->esq, out, idx);
    out[(*idx)++] = raiz->amostra;
    bst_emordem(raiz->dir, out, idx);
}

void bst_destruir(NoBST *raiz) {
    if (!raiz) return;
    bst_destruir(raiz->esq);
    bst_destruir(raiz->dir);
    free(raiz);
}
