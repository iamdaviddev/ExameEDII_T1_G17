#include "sigtag.h"

void iniciar(Sistema *s) {
    s->root_bst = NULL;
    s->root_avl = NULL;
    s->total_amostras = 0;
    s->comp_bst = 0;
    s->comp_avl = 0;
    s->comp_bubble = 0;
    s->comp_selection = 0;
    s->trocas_bubble = 0;
    s->trocas_selection = 0;
    giniciar(&s->grafo);
    menu_iniciar(&s->menu);
}

void popular_demo(Sistema *s) {
    /* Amostras pré-definidas */
    Amostra amostras[] = {
        {101, "AM-Luanda-01",  "Malanje",      "Diamante",   18.5f, 1, "10/06/2025"},
        {205, "AM-Cuando-05",  "Cuando Cubango","Ferro",      62.3f, 2, "12/06/2025"},
        {133, "AM-Moxico-03",  "Moxico",        "Ouro",       4.8f,  1, "08/06/2025"},
        {310, "AM-Cabinda-10", "Cabinda",       "Petroleo",   88.1f, 2, "15/06/2025"},
        {047, "AM-Huila-02",   "Huila",         "Calcario",   75.0f, 3, "05/06/2025"},
        {88, "AM-Benguela-08","Benguela",       "Manganes",  43.2f,  2, "11/06/2025"},
        {222, "AM-Huambo-04",  "Huambo",        "Quartzito",  30.0f, 3, "09/06/2025"},
        {175, "AM-Bie-07",     "Bie",            "Cobre",      55.7f, 1, "13/06/2025"},
        {390, "AM-Uige-09",    "Uige",           "Cobalto",    22.4f, 2, "14/06/2025"},
        {065, "AM-Lunda-06",   "Lunda Norte",   "Diamante",   9.6f,  1, "07/06/2025"},
    };

    int n = sizeof(amostras) / sizeof(amostras[0]);
    long long c = 0;
    for (int i = 0; i < n; i++) {
        s->root_bst = bst_inserir(s->root_bst, amostras[i], &c);
        s->root_avl = avl_inserir(s->root_avl, amostras[i], &c);
        if (s->total_amostras < MAX_AMOSTRAS)
            s->lista[s->total_amostras++] = amostras[i];
    }
    s->comp_bst = 0;
    s->comp_avl = 0;

    int luanda = gadd_ponto(&s->grafo, "Luanda (Lab. Central)", LABORATORIO);
    int malanje = gadd_ponto(&s->grafo, "Malanje (Extracao)", EXTRACAO);
    int benguela = gadd_ponto(&s->grafo, "Benguela (Extracao)", EXTRACAO);
    int huambo = gadd_ponto(&s->grafo, "Huambo (Lab. Regional)", LABORATORIO);
    int lubango = gadd_ponto(&s->grafo, "Lubango (Extracao)", EXTRACAO);
    int cabinda = gadd_ponto(&s->grafo, "Cabinda (Exportacao)", EXPORTACAO);
    int lobito = gadd_ponto(&s->grafo, "Lobito (Exportacao)", EXPORTACAO);

    gadd_aresta(&s->grafo, malanje, luanda, 350);
    gadd_aresta(&s->grafo, luanda, cabinda, 600);
    gadd_aresta(&s->grafo, luanda, benguela, 500);
    gadd_aresta(&s->grafo, benguela, huambo, 180);
    gadd_aresta(&s->grafo, huambo, lubango, 250);
    gadd_aresta(&s->grafo, benguela, lobito, 30);
    gadd_aresta(&s->grafo, malanje, huambo, 430);
    gadd_aresta(&s->grafo, lubango, lobito, 340);
}

/* ======================================================
   OPÇÃO 1 — INSERIR AMOSTRA
   ====================================================== */
void inserir_amostra(Sistema *s) {
    imprimir_separador("OPCAO 1 — Inserir Nova Amostra Geologica");
    if (s->total_amostras >= MAX_AMOSTRAS) {
        printf("  Sistema cheio! Limite de %d amostras atingido.\n", MAX_AMOSTRAS);
        pausar(); return;
    }
    Amostra a;
    a.codigo = ler_inteiro("Codigo da amostra", 1, 99999);

    /* Verifica duplicado */
    long long c = 0;
    if (bst_buscar(s->root_bst, a.codigo, &c)) {
        printf("  ERRO: Ja existe amostra com codigo %d.\n", a.codigo);
        pausar(); return;
    }

    getchar(); /* consome newline */
    ler_string("Nome da amostra",     a.nome,           MAX_NOME);
    ler_string("Local de extracao",   a.local_extracao, MAX_LOCAL);
    ler_string("Mineral principal",   a.mineral,        MAX_MINERAL);
    a.teor       = ler_float("Teor mineral (%)", 0.0f, 100.0f);
    a.prioridade = ler_inteiro("Prioridade (1=Alta 2=Media 3=Baixa)", 1, 3);
    ler_string("Data (DD/MM/AAAA)",   a.data, 12);

    long long cb = 0, ca = 0;
    s->root_bst = bst_inserir(s->root_bst, a, &cb);
    s->root_avl = avl_inserir(s->root_avl, a, &ca);
    s->lista[s->total_amostras++] = a;
    s->comp_bst += cb;
    s->comp_avl += ca;

    printf("\n  [OK] Amostra %d inserida com sucesso!\n", a.codigo);
    printf("       BST: %lld comparacoes | AVL: %lld comparacoes\n", cb, ca);
    printf("       Altura AVL atual: %d\n", avl_altura(s->root_avl));
    pausar();
}

/* ======================================================
   OPÇÃO 2 — BUSCAR AMOSTRA
   ====================================================== */
void buscar_amostra(Sistema *s) {
    imprimir_separador("OPCAO 2 — Buscar Amostra por Codigo");
    int cod = ler_inteiro("Codigo a buscar", 1, 99999);

    long long cb = 0, ca = 0;
    clock_t t0, t1;

    t0 = clock();
    NoBST *rb = bst_buscar(s->root_bst, cod, &cb);
    t1 = clock();
    double tempo_bst = (double)(t1 - t0) / CLOCKS_PER_SEC * 1e6;

    t0 = clock();
    avl_buscar(s->root_avl, cod, &ca);
    t1 = clock();
    double tempo_avl = (double)(t1 - t0) / CLOCKS_PER_SEC * 1e6;

    s->comp_bst += cb;
    s->comp_avl += ca;

    if (!rb) {
        printf("\n  Amostra com codigo %d nao encontrada.\n", cod);
    } else {
        printf("\n  === AMOSTRA ENCONTRADA ===\n");
        imprimir_amostra(&rb->amostra);
    }
    printf("\n  --- Desempenho da busca ---\n");
    printf("  BST : %lld comparacoes  (~%.1f us)\n", cb, tempo_bst);
    printf("  AVL : %lld comparacoes  (~%.1f us)\n", ca, tempo_avl);
    printf("  Altura AVL: %d  |  Estrutura: %s\n",
           avl_altura(s->root_avl),
           ca <= cb ? "AVL mais eficiente (balanceada)" : "BST foi mais rapida nesta busca");
    pausar();
}

/* ======================================================
   OPÇÃO 3 — REMOVER AMOSTRA
   ====================================================== */
void remover_amostra(Sistema *s) {
    imprimir_separador("OPCAO 3 — Remover Amostra Processada");
    int cod = ler_inteiro("Codigo da amostra a remover", 1, 99999);

    long long cb = 0, ca = 0;
    NoBST *rb = bst_buscar(s->root_bst, cod, &cb);
    if (!rb) {
        printf("\n  Amostra %d nao encontrada.\n", cod);
        pausar(); return;
    }

    s->root_bst = bst_remover(s->root_bst, cod, &cb);
    s->root_avl = avl_remover(s->root_avl, cod, &ca);

    /* Remove da lista plana */
    for (int i = 0; i < s->total_amostras; i++) {
        if (s->lista[i].codigo == cod) {
            s->lista[i] = s->lista[--s->total_amostras];
            break;
        }
    }
    s->comp_bst += cb; s->comp_avl += ca;
    printf("\n  [OK] Amostra %d removida. BST: %lld comp. | AVL: %lld comp.\n", cod, cb, ca);
    pausar();
}

/* ======================================================
   OPÇÃO 4 — LISTAGEM IN-ORDER
   ====================================================== */
void listar_inorder(Sistema *s) {
    imprimir_separador("OPCAO 4 — Listagem In-Order (ordem por codigo)");
    if (!s->root_bst) {
        printf("  Nenhuma amostra registada.\n");
        pausar(); return;
    }
    Amostra buf[MAX_AMOSTRAS];
    int idx = 0;
    avl_emordem(s->root_avl, buf, &idx);

    printf("  %-6s %-20s %-15s %-12s %6s %4s\n",
           "Cod", "Nome", "Local", "Mineral", "Teor%", "Prio");
    printf("  %s\n", "-------------------------------------------------------------------");
    for (int i = 0; i < idx; i++) {
        const char *p[] = {"Alta","Med","Bxa"};
        printf("  %-6d %-20s %-15s %-12s %6.2f %4s\n",
               buf[i].codigo, buf[i].nome, buf[i].local_extracao,
               buf[i].mineral, buf[i].teor,
               (buf[i].prioridade >= 1 && buf[i].prioridade <= 3)
                   ? p[buf[i].prioridade - 1] : "?");
    }
    printf("\n  Total: %d amostras\n", idx);
    pausar();
}

/* ======================================================
   OPÇÃO 5 — BUBBLE SORT POR TEOR
   ====================================================== */
void ordenar_bubble(Sistema *s) {
    imprimir_separador("OPCAO 5 — Bubble Sort por Teor Mineral");
    if (s->total_amostras == 0) {
        printf("  Sem amostras para ordenar.\n"); pausar(); return;
    }
    Amostra copia[MAX_AMOSTRAS];
    memcpy(copia, s->lista, s->total_amostras * sizeof(Amostra));

    long long comp = 0, trocas = 0;
    clock_t t0 = clock();
    bubble_sort_teor(copia, s->total_amostras, &comp, &trocas);
    clock_t t1 = clock();
    double tempo = (double)(t1 - t0) / CLOCKS_PER_SEC * 1e6;

    s->comp_bubble   = comp;
    s->trocas_bubble = trocas;

    printf("  Resultado (ordenado por teor mineral crescente):\n\n");
    printf("  %-6s %-20s %-12s %8s\n", "Cod", "Nome", "Mineral", "Teor%");
    printf("  %s\n", "--------------------------------------------------");
    for (int i = 0; i < s->total_amostras; i++)
        printf("  %-6d %-20s %-12s %8.2f%%\n",
               copia[i].codigo, copia[i].nome, copia[i].mineral, copia[i].teor);

    printf("\n  Comparacoes: %lld | Trocas: %lld | Tempo: %.1f us\n", comp, trocas, tempo);
    pausar();
}

/* ======================================================
   OPÇÃO 6 — SELECTION SORT POR PRIORIDADE
   ====================================================== */
void ordenar_selection(Sistema *s) {
    imprimir_separador("OPCAO 6 — Selection Sort por Prioridade");
    if (s->total_amostras == 0) {
        printf("  Sem amostras para ordenar.\n"); pausar(); return;
    }
    Amostra copia[MAX_AMOSTRAS];
    memcpy(copia, s->lista, s->total_amostras * sizeof(Amostra));

    long long comp = 0, trocas = 0;
    clock_t t0 = clock();
    selection_sort_prioridade(copia, s->total_amostras, &comp, &trocas);
    clock_t t1 = clock();
    double tempo = (double)(t1 - t0) / CLOCKS_PER_SEC * 1e6;

    s->comp_selection   = comp;
    s->trocas_selection = trocas;

    const char *prio[] = {"Alta", "Media", "Baixa"};
    printf("  Resultado (ordenado por prioridade crescente):\n\n");
    printf("  %-6s %-20s %-12s %8s\n", "Cod", "Nome", "Mineral", "Prio");
    printf("  %s\n", "--------------------------------------------------");
    for (int i = 0; i < s->total_amostras; i++)
        printf("  %-6d %-20s %-12s %8s\n",
               copia[i].codigo, copia[i].nome, copia[i].mineral,
               (copia[i].prioridade >= 1 && copia[i].prioridade <= 3)
                   ? prio[copia[i].prioridade - 1] : "?");

    printf("\n  Comparacoes: %lld | Trocas: %lld | Tempo: %.1f us\n", comp, trocas, tempo);
    pausar();
}

/* ======================================================
   OPÇÃO 7 — COMPARAR BST VS AVL
   ====================================================== */
void comparar_arvores(Sistema *s) {
    imprimir_separador("OPCAO 7 — Comparacao BST vs AVL");
    printf("  Testando com %d amostras já inseridas...\n\n", s->total_amostras);

    /* Reconstrói do zero para medir com precisão */
    NoBST *bst = NULL; NoAVL *avl = NULL;
    long long cb = 0;
    clock_t t0, t1;

    t0 = clock();
    for (int i = 0; i < s->total_amostras; i++)
        bst = bst_inserir(bst, s->lista[i], &cb);
    t1 = clock();
    double t_ins_bst = (double)(t1 - t0) / CLOCKS_PER_SEC * 1e6;

    long long ca2 = 0;
    t0 = clock();
    for (int i = 0; i < s->total_amostras; i++)
        avl = avl_inserir(avl, s->lista[i], &ca2);
    t1 = clock();
    double t_ins_avl = (double)(t1 - t0) / CLOCKS_PER_SEC * 1e6;

    /* Busca de 3 elementos */
    long long cb2 = 0, ca3 = 0;
    for (int i = 0; i < s->total_amostras; i++) {
        bst_buscar(bst, s->lista[i].codigo, &cb2);
        avl_buscar(avl, s->lista[i].codigo, &ca3);
    }

    printf("  +---------------------------+----------+----------+\n");
    printf("  | Operacao                  |    BST   |    AVL   |\n");
    printf("  +---------------------------+----------+----------+\n");
    printf("  | Insercoes (%3d amostras)  | %8lld | %8lld |\n",
           s->total_amostras, cb, ca2);
    printf("  | Tempo insercao total (us) | %8.1f | %8.1f |\n",
           t_ins_bst, t_ins_avl);
    printf("  | Buscas (%3d amostras)    | %8lld | %8lld |\n",
           s->total_amostras, cb2, ca3);
    printf("  | Altura atual              | %8s | %8d |\n",
           "N/A", avl_altura(avl));
    printf("  +---------------------------+----------+----------+\n");
    printf("\n  Conclusao: A AVL garante altura O(log n) = ~%d niveis.\n",
           avl_altura(avl));
    printf("  Em cenarios com insercoes sequenciais, a BST pode\n");
    printf("  degenerar para O(n), enquanto a AVL mantém O(log n).\n");

    bst_destruir(bst); avl_destruir(avl);
    pausar();
}

/* ======================================================
   OPÇÃO 8 — COMPARAR BUBBLE VS SELECTION
   ====================================================== */
void comparar_ordenacao(Sistema *s) {
    imprimir_separador("OPCAO 8 — Comparacao Bubble Sort vs Selection Sort");

    int tamanhos[] = {5, 10, s->total_amostras};
    int num_tests  = (s->total_amostras > 0) ? 3 : 2;

    printf("  +--------+--------------------+--------------------+\n");
    printf("  |   N    | Bubble Sort        | Selection Sort     |\n");
    printf("  |        | Comp.   | Trocas   | Comp.   | Trocas  |\n");
    printf("  +--------+---------+----------+---------+---------+\n");

    for (int t = 0; t < num_tests; t++) {
        int n = tamanhos[t];
        if (n <= 0 || n > s->total_amostras) continue;

        Amostra b[MAX_AMOSTRAS], sel[MAX_AMOSTRAS];
        memcpy(b,   s->lista, n * sizeof(Amostra));
        memcpy(sel, s->lista, n * sizeof(Amostra));

        long long cb=0, tb=0, cs=0, ts=0;
        bubble_sort_teor(b, n, &cb, &tb);
        selection_sort_prioridade(sel, n, &cs, &ts);

        printf("  | %6d | %7lld | %8lld | %7lld | %7lld |\n",
               n, cb, tb, cs, ts);
    }
    printf("  +--------+---------+----------+---------+---------+\n");
    printf("\n  Bubble Sort:    O(n²) comparacoes e O(n²) trocas no pior caso\n");
    printf("  Selection Sort: O(n²) comparacoes mas apenas O(n) trocas\n");
    printf("  => Selection Sort e preferivel quando trocas sao custosas.\n");
    pausar();
}

/* ======================================================
   OPÇÃO 9 — ADICIONAR PONTO AO GRAFO
   ====================================================== */
void add_ponto_grafo(Sistema *s) {
    imprimir_separador("OPCAO 9 — Adicionar Ponto a Rede Logistica");
    gimprimir(&s->grafo);

    if (s->grafo.n_pontos >= MAX_NOS_GRAFO) {
        printf("\n  Limite de pontos atingido.\n"); pausar(); return;
    }

    getchar();
    char nome[MAX_NOME];
    ler_string("Nome do ponto", nome, MAX_NOME);
    int tipo = ler_inteiro("Tipo (0=Extracao 1=Laboratorio 2=Exportacao)", 0, 2);
    int id   = gadd_ponto(&s->grafo, nome, (TipoPonto)tipo);

    /* Ligações */
    printf("\n  Ponto adicionado com ID %d.\n", id);
    printf("  Deseja ligar a um ponto existente? (0=Nao, ID do ponto destino): ");
    int dest;
    scanf("%d", &dest);
    if (dest > 0 && dest < s->grafo.n_pontos && dest != id) {
        int km = ler_inteiro("Distancia em km", 1, 9999);
        gadd_aresta(&s->grafo, id, dest, km);
        printf("  Ligacao adicionada!\n");
    }
    getchar();
    pausar();
}

/* ======================================================
   OPÇÃO 10 — CALCULAR ROTA
   ====================================================== */
void calcular_rota(Sistema *s) {
    imprimir_separador("OPCAO 10 — Calcular Rota Mais Eficiente");
    if (s->grafo.n_pontos < 2) {
        printf("  Rede logistica insuficiente (minimo 2 pontos).\n");
        pausar(); return;
    }
    gimprimir(&s->grafo);

    int orig = ler_inteiro("ID do ponto de ORIGEM",  0, s->grafo.n_pontos - 1);
    int dest = ler_inteiro("ID do ponto de DESTINO", 0, s->grafo.n_pontos - 1);

    int caminho[MAX_NOS_GRAFO], tam = 0, distancia = 0;
    gdijkstra(&s->grafo, orig, dest, caminho, &tam, &distancia);

    if (tam == 0) {
        printf("\n  Nao existe rota entre os pontos selecionados.\n");
    } else {
        printf("\n  === ROTA OTIMA (Dijkstra) ===\n");
        for (int i = 0; i < tam; i++) {
            printf("  %s", s->grafo.pontos[caminho[i]].nome);
            if (i < tam - 1) printf(" ---> ");
        }
        printf("\n\n  Distancia total: %d km\n", distancia);
    }
    pausar();
}

/* ======================================================
   OPÇÃO 11 — SUGESTÃO AUTOMÁTICA DE ESTRUTURA
   ====================================================== */
void sugerir_estrutura(Sistema *s) {
    imprimir_separador("OPCAO 11 — Sugestao Automatica de Estrutura Ideal");
    printf("  Analise do estado actual do sistema:\n\n");

    int n       = s->total_amostras;
    int alt_avl = avl_altura(s->root_avl);
    int ideal   = 0; { int tmp = n + 1; while (tmp > 1) { ideal++; tmp >>= 1; } ideal = (int)(ideal * 1.5f); }

    printf("  Amostras no sistema : %d\n", n);
    printf("  Altura AVL actual   : %d  (ideal log2(%d) ~ %d)\n", alt_avl, n, ideal);
    printf("  Comparacoes BST     : %lld\n", s->comp_bst);
    printf("  Comparacoes AVL     : %lld\n", s->comp_avl);

    printf("\n  --- RECOMENDACAO ---\n");
    if (n < 10)
        printf("  Com poucos dados, BST e AVL sao equivalentes.\n"
               "  Prefira BST pela menor sobrecarga de rotacoes.\n");
    else if (alt_avl <= ideal + 1)
        printf("  A AVL esta bem balanceada (altura %d, ideal ~%d).\n"
               "  Continue a usar AVL para insercoes/remocoes frequentes.\n",
               alt_avl, ideal);
    else
        printf("  A BST pode estar a degenerar. A AVL e fortemente recomendada\n"
               "  para manutencao de desempenho O(log n) nas buscas.\n");

    printf("\n  Para ORDENACAO:\n");
    if (n <= 20)
        printf("  Com %d amostras, Bubble e Selection Sort sao aceitaveis.\n", n);
    else
        printf("  Com %d amostras, considere algoritmos O(n log n)\n"
               "  como Merge Sort ou Quick Sort para maior eficiencia.\n", n);

    printf("\n  Para ROTAS LOGISTICAS:\n");
    printf("  Dijkstra e ideal para grafos com pesos positivos (distancias).\n");
    printf("  Numero de pontos na rede: %d\n", s->grafo.n_pontos);
    pausar();
}

/* ======================================================
   OPÇÃO 12 — ESTATÍSTICAS
   ====================================================== */
void ver_estatisticas(Sistema *s) {
    imprimir_separador("OPCAO 12 — Estatisticas de Uso do Sistema");
    printf("  === Estado das Estruturas ===\n");
    printf("  Amostras registadas  : %d\n", s->total_amostras);
    printf("  Altura AVL           : %d\n", avl_altura(s->root_avl));
    printf("  Pontos na rede       : %d\n", s->grafo.n_pontos);
    printf("  Arestas na rede      : %d\n", s->grafo.n_arestas);

    printf("\n  === Acumulado de Desempenho ===\n");
    printf("  Comparacoes BST      : %lld\n", s->comp_bst);
    printf("  Comparacoes AVL      : %lld\n", s->comp_avl);
    printf("  Comparacoes Bubble   : %lld  (Trocas: %lld)\n",
           s->comp_bubble, s->trocas_bubble);
    printf("  Comparacoes Selection: %lld  (Trocas: %lld)\n",
           s->comp_selection, s->trocas_selection);

    printf("\n  === Uso das Opcoes do Menu ===\n");
    for (int i = 0; i < NUM_OPCOES; i++) {
        printf("  [%2d] %-42s : %d usos\n",
               s->menu.opcoes[i].id,
               s->menu.opcoes[i].descricao,
               s->menu.opcoes[i].usos);
    }
    pausar();
}

/* ======================================================
   LOOP PRINCIPAL
   ====================================================== */
void executar(Sistema *s) {
    int opcao;
    do {
        limpar_tela();
        menu_imprimir(&s->menu);
        opcao = ler_inteiro("Escolha uma opcao", 1, NUM_OPCOES);
        menu_registar_uso(&s->menu, opcao);

        switch (opcao) {
            case  1: inserir_amostra(s);   break;
            case  2: buscar_amostra(s);    break;
            case  3: remover_amostra(s);   break;
            case  4: listar_inorder(s);    break;
            case  5: ordenar_bubble(s);    break;
            case  6: ordenar_selection(s); break;
            case  7: comparar_arvores(s);  break;
            case  8: comparar_ordenacao(s); break;
            case  9: add_ponto_grafo(s);   break;
            case 10: calcular_rota(s);     break;
            case 11: sugerir_estrutura(s); break;
            case 12: ver_estatisticas(s);  break;
            case 13: printf("\n  Encerrando SIGTAG. Ate logo!\n\n"); break;
        }
    } while (opcao != 13);

    /* Liberta memória */
    bst_destruir(s->root_bst);
    avl_destruir(s->root_avl);
}
