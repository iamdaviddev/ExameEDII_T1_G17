#include "include/rede.h"

int main(void) {
    Rede             rede;
    ListaPalavrasSpam spam;
    
    inicializar_rede(&rede);
    inicializar_lista_spam(&spam, 2);  // Por padrao, k = 2

    menu_principal(&rede, &spam);

    // Libera a memória que foi alocada
    for (int i = 0; i < rede.num_membros; i++) {
        // Remove os links de amizade
        NoAdj *p = rede.adj[i];
        while (p) {
            NoAdj *tmp = p->prox;
            free(p);
            p = tmp;
        }
        // Remove as filas de mensagens
        libertar_fila(&rede.membros[i].inbox);
        libertar_fila(&rede.membros[i].spam_box);
    }
    libertar_lista_spam(&spam);

    return 0;
}
