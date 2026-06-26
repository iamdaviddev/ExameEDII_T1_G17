#include "structs.h"

int main() {
    printf("=== SIMULADOR DE REDE DE COMPUTADORES ===\n\n");

    // Cria uma rede com capacidade para 5 utilizadores
    RedeComputadores* minha_rede = criar_rede(5);

    // Cadastra 4 utilizadores (Limite de 3 mensagens nas caixas)
    cadastrar_usuario(minha_rede, 1, "Alice", 3);
    cadastrar_usuario(minha_rede, 2, "Bob", 3);
    cadastrar_usuario(minha_rede, 3, "Charlie", 3);
    cadastrar_usuario(minha_rede, 4, "David", 3);

    printf("[+] Utilizadores cadastrados com sucesso!\n");

    // Cria as ligações (Arestas do Grafo)
    // Alice (1) <-> Bob (2)
    // Bob (2)   <-> Charlie (3)
    conectar_amigos(minha_rede, 1, 2);
    conectar_amigos(minha_rede, 2, 3);

    printf("[+] Ligações criadas: Alice--Bob e Bob--Charlie\n\n");

    // Testa a regra de vizinhos diretos (Quem pode falar com quem)
    printf("=== TESTE DE CONECTIVIDADE ===\n");
    
    // Teste 1: Alice e Bob (Devem ser vizinhos)
    if (sao_vizinhos_diretos(minha_rede, 1, 2)) {
        printf("-> Alice e Bob PODEM comunicar diretamente. (Correto)\n");
    } else {
        printf("-> Erro no teste de Alice e Bob.\n");
    }

    // Teste 2: Alice e Charlie (NÃO devem ser vizinhos diretos, precisam do Bob)
    if (sao_vizinhos_diretos(minha_rede, 1, 3)) {
        printf("-> Erro: Alice e Charlie nao deviam comunicar diretamente.\n");
    } else {
        printf("-> Alice e Charlie NAO PODEM comunicar diretamente. (Correto, o enunciado proíbe)\n");
    }

    // Teste 3: David (Está isolado na rede)
    if (sao_vizinhos_diretos(minha_rede, 2, 4)) {
        printf("-> Erro: Bob e David nao sao amigos.\n");
    } else {
        printf("-> Bob e David NAO PODEM comunicar diretamente. (Correto)\n");
    }

    // Libera a memória antes de fechar
    destruir_rede(minha_rede);
    printf("\n[+] Memoria libertada. Programa terminado com sucesso.\n");

    return 0;
}