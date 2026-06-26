#include "sigtag.h"

/* ======================================================
   BUBBLE SORT — ordenação por teor mineral (crescente)
   ====================================================== */
void bubble_sort_teor(Amostra *arr, int n, long long *comp, long long *trocas) {
    *comp   = 0;
    *trocas = 0;
    int i, j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            (*comp)++;
            if (arr[j].teor > arr[j + 1].teor) {
                Amostra tmp  = arr[j];
                arr[j]       = arr[j + 1];
                arr[j + 1]   = tmp;
                (*trocas)++;
            }
        }
    }
}

/* ======================================================
   SELECTION SORT — ordenação por prioridade (crescente)
   ====================================================== */
void selection_sort_prioridade(Amostra *arr, int n, long long *comp, long long *trocas) {
    *comp   = 0;
    *trocas = 0;
    int i, j, min_idx;
    for (i = 0; i < n - 1; i++) {
        min_idx = i;
        for (j = i + 1; j < n; j++) {
            (*comp)++;
            if (arr[j].prioridade < arr[min_idx].prioridade)
                min_idx = j;
        }
        if (min_idx != i) {
            Amostra tmp    = arr[i];
            arr[i]         = arr[min_idx];
            arr[min_idx]   = tmp;
            (*trocas)++;
        }
    }
}
