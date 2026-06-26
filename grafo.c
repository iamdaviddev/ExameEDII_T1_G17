#include "sigtag.h"

#define INF 999999

void giniciar(Grafo *g) {
    g->n_pontos = 0;
    g->n_arestas = 0;
    for (int i = 0; i < MAX_NOS_GRAFO; i++)
        for (int j = 0; j < MAX_NOS_GRAFO; j++)
            g->adj[i][j] = -1;
}

int gadd_ponto(Grafo *g, const char *nome, TipoPonto tipo) {
    if (g->n_pontos >= MAX_NOS_GRAFO) {
        printf("Limite de pontos atingido!\n");
        return -1;
    }
    int id = g->n_pontos;
    g->pontos[id].id = id;
    g->pontos[id].tipo = tipo;
    strncpy(g->pontos[id].nome, nome, MAX_NOME - 1);
    g->pontos[id].nome[MAX_NOME - 1] = '\0';
    g->n_pontos++;
    return id;
}

void gadd_aresta(Grafo *g, int orig, int dest, int peso) {
    if (orig < 0 || dest < 0 || orig >= g->n_pontos || dest >= g->n_pontos) return;
    g->adj[orig][dest] = peso;
    g->adj[dest][orig] = peso;
    g->n_arestas++;
}

void gdijkstra(Grafo *g, int origem, int destino,
               int *caminho, int *tam_caminho, int *distancia) {
    int n = g->n_pontos;
    int dist[MAX_NOS_GRAFO], prev[MAX_NOS_GRAFO], visitado[MAX_NOS_GRAFO];

    for (int i = 0; i < n; i++) {
        dist[i] = INF;
        prev[i] = -1;
        visitado[i] = 0;
    }
    dist[origem] = 0;

    for (int iter = 0; iter < n - 1; iter++) {
        int u = -1;
        for (int i = 0; i < n; i++)
            if (!visitado[i] && (u == -1 || dist[i] < dist[u]))
                u = i;
        if (u == -1 || dist[u] == INF) break;
        visitado[u] = 1;

        for (int v = 0; v < n; v++) {
            if (g->adj[u][v] >= 0 && !visitado[v]) {
                int novo = dist[u] + g->adj[u][v];
                if (novo < dist[v]) {
                    dist[v] = novo;
                    prev[v] = u;
                }
            }
        }
    }

    *distancia = dist[destino];
    *tam_caminho = 0;

    if (dist[destino] == INF) return;

    int tmp[MAX_NOS_GRAFO], t = 0, cur = destino;
    while (cur != -1) {
        tmp[t++] = cur;
        cur = prev[cur];
    }
    for (int i = 0; i < t; i++)
        caminho[i] = tmp[t - 1 - i];
    *tam_caminho = t;
}

void gimprimir(Grafo *g) {
    const char *tipos[] = {"EXTRAÇÃO", "LABORATÓRIO", "EXPORTAÇÃO"};
    printf("\n  %-4s %-30s %-15s\n", "ID", "Nome", "Tipo");
    printf("  %s\n", "------------------------------------------------------------");
    for (int i = 0; i < g->n_pontos; i++) {
        printf("  [%2d] %-30s %s\n",
               g->pontos[i].id,
               g->pontos[i].nome,
               tipos[g->pontos[i].tipo]);
    }
    printf("\n  Ligações (km):\n");
    for (int i = 0; i < g->n_pontos; i++)
        for (int j = i + 1; j < g->n_pontos; j++)
            if (g->adj[i][j] >= 0)
                printf("  %s ↔ %s : %d km\n",
                       g->pontos[i].nome,
                       g->pontos[j].nome,
                       g->adj[i][j]);
}
