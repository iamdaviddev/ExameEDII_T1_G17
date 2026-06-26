#include "sigtag.h"

int avl_altura(NoAVL *no) {
    return no ? no->altura : 0;
}

static int max2(int a, int b) { return a > b ? a : b; }

static int fator_balanceamento(NoAVL *no) {
    return no ? avl_altura(no->esq) - avl_altura(no->dir) : 0;
}

static void atualizar_altura(NoAVL *no) {
    if (no)
        no->altura = 1 + max2(avl_altura(no->esq), avl_altura(no->dir));
}

static NoAVL* novo_no_avl(Amostra a) {
    NoAVL *no = (NoAVL*)malloc(sizeof(NoAVL));
    if (!no) {
        fprintf(stderr, "Erro: sem memória (AVL)\n");
        exit(1);
    }
    no->amostra = a;
    no->esq = no->dir = NULL;
    no->altura = 1;
    return no;
}

static NoAVL* rotacao_direita(NoAVL *y) {
    NoAVL *x  = y->esq;
    NoAVL *T2 = x->dir;
    x->dir = y;
    y->esq = T2;
    atualizar_altura(y);
    atualizar_altura(x);
    return x;
}

static NoAVL* rotacao_esquerda(NoAVL *x) {
    NoAVL *y  = x->dir;
    NoAVL *T2 = y->esq;
    y->esq = x;
    x->dir = T2;
    atualizar_altura(x);
    atualizar_altura(y);
    return y;
}

static NoAVL* balancear(NoAVL *no) {
    atualizar_altura(no);
    int fb = fator_balanceamento(no);

    if (fb > 1 && fator_balanceamento(no->esq) >= 0)
        return rotacao_direita(no);
    if (fb > 1 && fator_balanceamento(no->esq) < 0) {
        no->esq = rotacao_esquerda(no->esq);
        return rotacao_direita(no);
    }
    if (fb < -1 && fator_balanceamento(no->dir) <= 0)
        return rotacao_esquerda(no);
    if (fb < -1 && fator_balanceamento(no->dir) > 0) {
        no->dir = rotacao_direita(no->dir);
        return rotacao_esquerda(no);
    }
    return no;
}

NoAVL* avl_inserir(NoAVL *raiz, Amostra a, long long *comp) {
    if (!raiz) return novo_no_avl(a);
    (*comp)++;
    if (a.codigo < raiz->amostra.codigo)
        raiz->esq = avl_inserir(raiz->esq, a, comp);
    else if (a.codigo > raiz->amostra.codigo)
        raiz->dir = avl_inserir(raiz->dir, a, comp);
    else
        return raiz;
    return balancear(raiz);
}

NoAVL* avl_buscar(NoAVL *raiz, int codigo, long long *comp) {
    if (!raiz) return NULL;
    (*comp)++;
    if (codigo == raiz->amostra.codigo) return raiz;
    if (codigo < raiz->amostra.codigo)
        return avl_buscar(raiz->esq, codigo, comp);
    return avl_buscar(raiz->dir, codigo, comp);
}

static NoAVL* avl_min(NoAVL *no) {
    while (no->esq) no = no->esq;
    return no;
}

NoAVL* avl_remover(NoAVL *raiz, int codigo, long long *comp) {
    if (!raiz) return NULL;
    (*comp)++;
    if (codigo < raiz->amostra.codigo)
        raiz->esq = avl_remover(raiz->esq, codigo, comp);
    else if (codigo > raiz->amostra.codigo)
        raiz->dir = avl_remover(raiz->dir, codigo, comp);
    else {
        if (!raiz->esq || !raiz->dir) {
            NoAVL *tmp = raiz->esq ? raiz->esq : raiz->dir;
            free(raiz);
            return tmp;
        }
        NoAVL *suc = avl_min(raiz->dir);
        raiz->amostra = suc->amostra;
        raiz->dir = avl_remover(raiz->dir, suc->amostra.codigo, comp);
    }
    return balancear(raiz);
}

void avl_emordem(NoAVL *raiz, Amostra *out, int *idx) {
    if (!raiz) return;
    avl_emordem(raiz->esq, out, idx);
    out[(*idx)++] = raiz->amostra;
    avl_emordem(raiz->dir, out, idx);
}

void avl_destruir(NoAVL *raiz) {
    if (!raiz) return;
    avl_destruir(raiz->esq);
    avl_destruir(raiz->dir);
    free(raiz);
}
