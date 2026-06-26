#include "sigtag.h"

void limpar_tela(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pausar(void) {
    printf("\n  Pressione ENTER para continuar...");
    while (getchar() != '\n');
}

void imprimir_separador(const char *titulo) {
    printf("\n  ============================================================\n");
    printf("   %s\n", titulo);
    printf("  ============================================================\n");
}

void imprimir_amostra(const Amostra *a) {
    const char *prio[] = {"Alta", "Media", "Baixa"};
    printf("  Codigo     : %d\n",   a->codigo);
    printf("  Nome       : %s\n",   a->nome);
    printf("  Local      : %s\n",   a->local_extracao);
    printf("  Mineral    : %s\n",   a->mineral);
    printf("  Teor       : %.2f%%\n", a->teor);
    printf("  Prioridade : %s\n",   (a->prioridade >= 1 && a->prioridade <= 3)
                                      ? prio[a->prioridade - 1] : "?");
    printf("  Data       : %s\n",   a->data);
}

int ler_inteiro(const char *prompt, int min, int max) {
    int val;
    char buf[64];
    while (1) {
        printf("  %s [%d-%d]: ", prompt, min, max);
        if (fgets(buf, sizeof(buf), stdin)) {
            if (sscanf(buf, "%d", &val) == 1 && val >= min && val <= max)
                return val;
        }
        printf("  Valor invalido. Tente novamente.\n");
    }
}

float ler_float(const char *prompt, float min, float max) {
    float val;
    char buf[64];
    while (1) {
        printf("  %s [%.1f-%.1f]: ", prompt, min, max);
        if (fgets(buf, sizeof(buf), stdin)) {
            if (sscanf(buf, "%f", &val) == 1 && val >= min && val <= max)
                return val;
        }
        printf("  Valor invalido. Tente novamente.\n");
    }
}

void ler_string(const char *prompt, char *dest, int tam) {
    printf("  %s: ", prompt);
    if (fgets(dest, tam, stdin)) {
        dest[strcspn(dest, "\n")] = '\0';
    }
}
