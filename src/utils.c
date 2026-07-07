#include "include/rede.h"

// Copia uma string transformando pra minúscula
void str_tolower_copy(char *dest, const char *src, int max) {
    int i;
    for (i = 0; i < max - 1 && src[i]; i++) {
        dest[i] = (char)tolower((unsigned char)src[i]);
    }
    dest[i] = '\0';
}

// Limpa o buffer de entrada depois que alguém digita algo
void limpar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Desenha uma linha divisória bonita no menu
static void imprimir_separador(void) {
    printf("\n+-----------------------------------------------+\n");
}

// Menu principal da aplicação pra interagir com tudo
void menu_principal(Rede *r, ListaPalavrasSpam *ls) {
    int opcao;
    char buf1[MAX_NOME], buf2[MAX_EMAIL], buf3[MAX_MENSAGEM];
    int  id1, id2;

    // Dados de exemplo pra testar
    adicionar_membro(r, "Alice",   "alice@rede.ao");
    adicionar_membro(r, "Bruno",   "bruno@rede.ao");
    adicionar_membro(r, "Carlos",  "carlos@rede.ao");
    adicionar_membro(r, "Diana",   "diana@rede.ao");
    adicionar_ligacao(r, 0, 1);   // Alice amiga de Bruno
    adicionar_ligacao(r, 0, 2);   // Alice amiga de Carlos
    adicionar_ligacao(r, 1, 3);   // Bruno amigo de Diana
    adicionar_ligacao(r, 2, 3);   // Carlos amigo de Diana

    // Palavras que indicam spam
    ls->k = 2;
    adicionar_palavra_spam(ls, "gratis");
    adicionar_palavra_spam(ls, "ganhe");
    adicionar_palavra_spam(ls, "promo");
    adicionar_palavra_spam(ls, "oferta");
    adicionar_palavra_spam(ls, "clique");

    do {
        imprimir_separador();
        printf("|        REDE DE EMAIL — MENU PRINCIPAL        |\n");
        imprimir_separador();
        printf("| 1. Adicionar membro                           |\n");
        printf("| 2. Adicionar ligacao                          |\n");
        printf("| 3. Listar membros                             |\n");
        printf("| 4. Listar ligacoes                            |\n");
        printf("| 5. Enviar mensagem                            |\n");
        printf("| 6. Ver caixa de um membro                     |\n");
        printf("| 7. Adicionar palavra spam                     |\n");
        printf("| 8. Listar palavras spam                       |\n");
        printf("| 9. Alterar valor de k                         |\n");
        printf("| 0. Sair                                       |\n");
        imprimir_separador();
        printf("Opcao: ");
        if (scanf("%d", &opcao) != 1) { limpar_buffer(); opcao = -1; }
        limpar_buffer();

        switch (opcao) {
            // Inscreve uma nova pessoa
            case 1:
                printf("Nome: ");
                fgets(buf1, MAX_NOME, stdin);
                buf1[strcspn(buf1, "\n")] = '\0';
                printf("Email: ");
                fgets(buf2, MAX_EMAIL, stdin);
                buf2[strcspn(buf2, "\n")] = '\0';
                adicionar_membro(r, buf1, buf2);
                break;

            // Cria uma amizade entre dois membros
            case 2:
                listar_membros(r);
                printf("Id membro 1: "); scanf("%d", &id1); limpar_buffer();
                printf("Id membro 2: "); scanf("%d", &id2); limpar_buffer();
                adicionar_ligacao(r, id1, id2);
                break;

            // Mostra quem tá na rede
            case 3:
                listar_membros(r);
                break;

            // Mostra quem é amigo de quem
            case 4:
                listar_ligacoes(r);
                break;

            // Manda uma mensagem
            case 5:
                listar_membros(r);
                printf("Id remetente:  "); scanf("%d", &id1); limpar_buffer();
                printf("Id destinatario: "); scanf("%d", &id2); limpar_buffer();
                printf("Mensagem: ");
                fgets(buf3, MAX_MENSAGEM, stdin);
                buf3[strcspn(buf3, "\n")] = '\0';
                enviar_mensagem(r, ls, id1, id2, buf3);
                break;

            // Ver as mensagens de alguém
            case 6:
                listar_membros(r);
                printf("Id do membro: "); scanf("%d", &id1); limpar_buffer();
                ver_caixa(r, id1);
                break;

            // Adiciona uma palavra pra detectar spam
            case 7:
                printf("Palavra: ");
                fgets(buf1, MAX_NOME, stdin);
                buf1[strcspn(buf1, "\n")] = '\0';
                adicionar_palavra_spam(ls, buf1);
                break;

            // Mostra as palavras que detectam spam
            case 8:
                listar_palavras_spam(ls);
                break;

            // Muda quantas palavras spam precisa ter pra ser considerado spam
            case 9:
                printf("Novo valor de k (minimo de palavras para ser spam): ");
                scanf("%d", &id1); limpar_buffer();
                if (id1 > 0) { ls->k = id1; printf("[OK] k = %d\n", ls->k); }
                else printf("[ERRO] k deve ser > 0.\n");
                break;

            case 0:
                printf("A sair...\n");
                break;

            default:
                printf("[ERRO] Opcao invalida.\n");
        }
    } while (opcao != 0);
}
