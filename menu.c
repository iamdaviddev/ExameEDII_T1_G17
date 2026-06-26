#include "sigtag.h"

static const char *descricoes_iniciais[NUM_OPCOES] = {
    "Inserir nova amostra geologica",
    "Buscar amostra por codigo",
    "Remover amostra processada",
    "Listar amostras em ordem",
    "Ordenar amostras por teor mineral",
    "Ordenar amostras por prioridade",
    "Comparar desempenho BST vs AVL",
    "Comparar desempenho Bubble vs Selection Sort",
    "Adicionar ponto a rede logistica",
    "Calcular rota mais eficiente ate o laboratorio",
    "Sugestao automatica de estrutura ideal",
    "Ver estatisticas de uso do sistema",
    "Sair do sistema"
};

void menu_iniciar(MenuAdaptativo *m) {
    for (int i = 0; i < NUM_OPCOES; i++) {
        m->opcoes[i].id = i + 1;
        m->opcoes[i].usos = 0;
        strncpy(m->opcoes[i].descricao, descricoes_iniciais[i], MAX_NOME - 1);
        m->opcoes[i].descricao[MAX_NOME - 1] = '\0';
    }
}

void menu_registar_uso(MenuAdaptativo *m, int id) {
    for (int i = 0; i < NUM_OPCOES; i++)
        if (m->opcoes[i].id == id) {
            m->opcoes[i].usos++;
            return;
        }
}

void menu_ordenar(MenuAdaptativo *m) {
    for (int i = 1; i < NUM_OPCOES - 1; i++) {
        OpcaoMenu chave = m->opcoes[i];
        int j = i - 1;
        while (j >= 0 && m->opcoes[j].usos < chave.usos) {
            m->opcoes[j + 1] = m->opcoes[j];
            j--;
        }
        m->opcoes[j + 1] = chave;
    }
}

void menu_imprimir(MenuAdaptativo *m) {
    menu_ordenar(m);
    printf("\n");
    printf("  +-------------------------------------------------------+\n");
    printf("  |       SIGTAG - Sistema Inteligente de Gestao          |\n");
    printf("  |       e Triagem de Amostras Geologicas                |\n");
    printf("  +-------------------------------------------------------+\n");
    printf("  | # |  Opcao                                    | Usos  |\n");
    printf("  +---+-------------------------------------------+-------+\n");
    for (int i = 0; i < NUM_OPCOES; i++) {
        printf("  |%2d | %-41s | %5d |\n",
               m->opcoes[i].id,
               m->opcoes[i].descricao,
               m->opcoes[i].usos);
    }
    printf("  +-------------------------------------------------------+\n");
    printf("  * As opcoes mais usadas aparecem primeiro automaticamente\n");
}
