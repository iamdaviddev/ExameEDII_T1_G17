#include "sigtag.h"

int ficheiro_ler_amostras(const char *caminho, Amostra *arr, int max) {
    FILE *f = fopen(caminho, "r");
    if (!f) {
        printf("  [AVISO] Ficheiro '%s' nao encontrado. A usar dados de demo.\n", caminho);
        return 0;
    }

    int count = 0;
    char linha[256];

    while (fgets(linha, sizeof(linha), f) && count < max) {
        if (linha[0] == '#' || linha[0] == '\n' || linha[0] == '\r') continue;

        Amostra a;
        if (sscanf(linha, "%d|%99[^|]|%99[^|]|%49[^|]|%f|%d|%11s",
                   &a.codigo, a.nome, a.local_extracao,
                   a.mineral, &a.teor, &a.prioridade, a.data) == 7) {
            arr[count++] = a;
        }
    }
    fclose(f);
    printf("  [OK] %d amostras lidas de '%s'\n", count, caminho);
    return count;
}

void ficheiro_ler_grafo(const char *caminho, Grafo *g) {
    FILE *f = fopen(caminho, "r");
    if (!f) {
        printf("  [AVISO] Ficheiro '%s' nao encontrado. A usar rede de demo.\n", caminho);
        return;
    }

    char linha[256];
    while (fgets(linha, sizeof(linha), f)) {
        if (linha[0] == '#' || linha[0] == '\n') continue;
        char tipo[10];
        sscanf(linha, "%9[^|\n]", tipo);
        if (strcmp(tipo, "PONTO") == 0) {
            int id, tp;
            char nome[MAX_NOME];
            if (sscanf(linha, "PONTO|%d|%99[^|]|%d", &id, nome, &tp) == 3)
                gadd_ponto(g, nome, (TipoPonto)tp);
        }
    }

    rewind(f);
    while (fgets(linha, sizeof(linha), f)) {
        if (linha[0] == '#' || linha[0] == '\n') continue;
        char tipo[10];
        sscanf(linha, "%9[^|\n]", tipo);
        if (strcmp(tipo, "ARESTA") == 0) {
            int orig, dest, peso;
            if (sscanf(linha, "ARESTA|%d|%d|%d", &orig, &dest, &peso) == 3)
                gadd_aresta(g, orig, dest, peso);
        }
    }
    fclose(f);
    printf("  [OK] Rede logistica lida de '%s' (%d pontos, %d arestas)\n",
           caminho, g->n_pontos, g->n_arestas);
}

void ficheiro_escrever_relatorio(const char *caminho, Sistema *s) {
    FILE *f = fopen(caminho, "w");
    if (!f) {
        printf("  [ERRO] Nao foi possivel criar '%s'\n", caminho);
        return;
    }

    fprintf(f, "=============================================================\n");
    fprintf(f, "  SIGTAG — Relatorio de Saida\n");
    fprintf(f, "  Gerado automaticamente pelo sistema\n");
    fprintf(f, "=============================================================\n\n");

    fprintf(f, "AMOSTRAS REGISTADAS (ordem por codigo):\n");
    fprintf(f, "%-6s %-20s %-15s %-12s %7s %4s\n",
            "Cod", "Nome", "Local", "Mineral", "Teor%", "Prio");
    fprintf(f, "---------------------------------------------------------------\n");

    Amostra buf[MAX_AMOSTRAS];
    int idx = 0;
    avl_emordem(s->root_avl, buf, &idx);
    const char *prio[] = {"Alta", "Med", "Bxa"};
    for (int i = 0; i < idx; i++) {
        fprintf(f, "%-6d %-20s %-15s %-12s %7.2f%% %4s\n",
                buf[i].codigo, buf[i].nome, buf[i].local_extracao,
                buf[i].mineral, buf[i].teor,
                (buf[i].prioridade >= 1 && buf[i].prioridade <= 3)
                    ? prio[buf[i].prioridade - 1] : "?");
    }

    fprintf(f, "\nESTATISTICAS DE DESEMPENHO:\n");
    fprintf(f, "  Total de amostras : %d\n", s->total_amostras);
    fprintf(f, "  Comparacoes BST   : %lld\n", s->comp_bst);
    fprintf(f, "  Comparacoes AVL   : %lld\n", s->comp_avl);
    fprintf(f, "  Altura AVL        : %d\n", avl_altura(s->root_avl));
    fprintf(f, "  Comp. Bubble Sort : %lld (Trocas: %lld)\n",
            s->comp_bubble, s->trocas_bubble);
    fprintf(f, "  Comp. Selection   : %lld (Trocas: %lld)\n",
            s->comp_selection, s->trocas_selection);

    fprintf(f, "\nREDE LOGISTICA (%d pontos, %d arestas):\n",
            s->grafo.n_pontos, s->grafo.n_arestas);
    const char *tipos[] = {"Extracao", "Laboratorio", "Exportacao"};
    for (int i = 0; i < s->grafo.n_pontos; i++)
        fprintf(f, "  [%d] %-30s  %s\n",
                s->grafo.pontos[i].id,
                s->grafo.pontos[i].nome,
                tipos[s->grafo.pontos[i].tipo]);

    fprintf(f, "\n=============================================================\n");
    fclose(f);
    printf("  [OK] Relatorio escrito em '%s'\n", caminho);
}
