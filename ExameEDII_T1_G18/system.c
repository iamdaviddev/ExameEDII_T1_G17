#include "structs.h"


/* Função que cria e aloca a estrutura principal da rede */
RedeComputadores* criar_rede(int capacidade) {
    RedeComputadores* rede = (RedeComputadores*) malloc(sizeof(RedeComputadores));
    if (rede == NULL) {
        printf("Erro ao alocar memória para a rede.\n");
        exit(1);
    }
    
    rede->usuarios = (Usuario*) malloc(capacidade * sizeof(Usuario));
    if (rede->usuarios == NULL) {
        printf("Erro ao alocar memória para os utilizadores.\n");
        free(rede);
        exit(1);
    }
    
    rede->total_usuarios = 0;
    rede->capacidade_maxima = capacidade;
    return rede;
}

// Função auxiliar para iniciar as caixas
void iniciar_caixa(CaixaCorreio* caixa, int limite) {
    caixa->frente = NULL;
    caixa->tras = NULL;
    caixa->total_atual = 0;
    caixa->limite_maximo = limite;
}

// Função para regista um novo usuário na rede (um novo nó no Grafo)
void cadastrar_usuario(RedeComputadores* rede, int id, char* nome, int limite_caixa) {
    if (rede->total_usuarios >= rede->capacidade_maxima) {
        printf("Erro: Capacidade máxima da rede atingida.\n");
        return;
    }
    
    int posicao = rede->total_usuarios;
    rede->usuarios[posicao].id = id;
    strcpy(rede->usuarios[posicao].nome, nome);
    rede->usuarios[posicao].lista_amigos = NULL;
    
    // Inicializa as caixas de correio do utilizador
    inicializar_caixa(&rede->usuarios[posicao].caixa_normal, limite_caixa);
    inicializar_caixa(&rede->usuarios[posicao].caixa_spam, limite_caixa);
    
    rede->total_usuarios++;
}

// Função auxiliar para encontrar a posição de um ID no vetor de usuários. Retorna -1 se não encontrado.
int encontrar_posicao_por_id(RedeComputadores* rede, int id) {
    for (int i = 0; i < rede->total_usuarios; i++) {
        if (rede->usuarios[i].id == id) {
            return i;
        }
    }
    return -1;
}

// Função que conecta dois amigos (Adiciona uma aresta não direcionada no Grafo)
void conectar_amigos(RedeComputadores* rede, int id1, int id2) {
    int pos1 = encontrar_posicao_por_id(rede, id1);
    int pos2 = encontrar_posicao_por_id(rede, id2);
    
    if (pos1 == -1 || pos2 == -1) {
        printf("Erro: Um ou ambos os utilizadores não existem.\n");
        return;
    }
    
    // Inserir no início da lista de adjacência do Usuário 1
    Vizinho* novo_vizinho1 = (Vizinho*) malloc(sizeof(Vizinho));
    novo_vizinho1->id_vizinho = id2;
    novo_vizinho1->proximo = rede->usuarios[pos1].lista_amigos;
    rede->usuarios[pos1].lista_amigos = novo_vizinho1;
    
    // Como a ligação é mútua, fazemos o mesmo para o Utilizador 2
    Vizinho* novo_vizinho2 = (Vizinho*) malloc(sizeof(Vizinho));
    novo_vizinho2->id_vizinho = id1;
    novo_vizinho2->proximo = rede->usuarios[pos2].lista_amigos;
    rede->usuarios[pos2].lista_amigos = novo_vizinho2;
}

// Função que verifica se existe uma ligação direta entre dois usuários
int sao_amigos_diretos(RedeComputadores* rede, int id1, int id2) {
    int pos1 = encontrar_posicao_por_id(rede, id1);
    if (pos1 == -1) return 0;
    
    Vizinho* atual = rede->usuarios[pos1].lista_amigos;
    while (atual != NULL) {
        if (atual->id_vizinho == id2) {
            return 1;
        }
        atual = atual->proximo;
    }
    return 0;
}