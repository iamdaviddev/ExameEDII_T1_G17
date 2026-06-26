#include "sigtag.h"

#define FICHEIRO_AMOSTRAS  "dados/amostras_entrada.txt"
#define FICHEIRO_GRAFO     "dados/rede_logistica.txt"
#define FICHEIRO_SAIDA     "dados/relatorio_saida.txt"

int main(void) {
    Sistema s;

    printf("\n");
    printf("  ============================================================\n");
    printf("   SIGTAG — Sistema Inteligente de Gestao e Triagem de\n");
    printf("            Amostras Geologicas\n");
    printf("  ============================================================\n\n");

    iniciar(&s);

    printf("  A carregar dados de entrada...\n");
    Amostra buf[MAX_AMOSTRAS];
    int lidas = ler_amostras(FICHEIRO_AMOSTRAS, buf, MAX_AMOSTRAS);

    if (lidas > 0) {
        long long c = 0;
        for (int i = 0; i < lidas; i++) {
            s.root_bst = bst_inserir(s.root_bst, buf[i], &c);
            s.root_avl = avl_inserir(s.root_avl, buf[i], &c);
            s.lista[s.total_amostras++] = buf[i];
        }
        ler_grafo(FICHEIRO_GRAFO, &s.grafo);
        s.comp_bst = s.comp_avl = 0;
    } else {
        printf("  A usar dados de demonstracao...\n");
        popular_demo(&s);
    }

    printf("\n  [OK] %d amostras no sistema | Altura AVL: %d\n",
           s.total_amostras, avl_altura(s.root_avl));
    printf("  [OK] Rede logistica: %d pontos, %d ligacoes\n\n",
           s.grafo.n_pontos, s.grafo.n_arestas);

    pausar();
    executar(&s);

    printf("\n  A gerar relatorio de saida...\n");
    escrever_relatorio(FICHEIRO_SAIDA, &s);

    printf("  Memoria libertada. Ate logo!\n\n");
    return 0;
}
