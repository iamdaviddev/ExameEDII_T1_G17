const {
  Document, Packer, Paragraph, TextRun, Table, TableRow, TableCell,
  HeadingLevel, AlignmentType, BorderStyle, WidthType, ShadingType,
  LevelFormat, PageNumber, PageBreak, Footer, Header
} = require('docx');
const fs = require('fs');

/* ---- helpers ---- */
const border = { style: BorderStyle.SINGLE, size: 1, color: "CCCCCC" };
const borders = { top: border, bottom: border, left: border, right: border };
const cellM = { top: 100, bottom: 100, left: 140, right: 140 };

function h1(text) {
  return new Paragraph({ heading: HeadingLevel.HEADING_1, children: [new TextRun({ text, bold: true })] });
}
function h2(text) {
  return new Paragraph({ heading: HeadingLevel.HEADING_2, children: [new TextRun({ text, bold: true })] });
}
function h3(text) {
  return new Paragraph({ heading: HeadingLevel.HEADING_3, children: [new TextRun({ text, bold: true })] });
}
function p(text, opts = {}) {
  return new Paragraph({ children: [new TextRun({ text, ...opts })], spacing: { after: 140 } });
}
function bullet(text) {
  return new Paragraph({
    numbering: { reference: "bullets", level: 0 },
    children: [new TextRun({ text })],
    spacing: { after: 80 }
  });
}
function code(text) {
  return new Paragraph({
    children: [new TextRun({ text, font: "Courier New", size: 18, color: "1E3A5F" })],
    spacing: { after: 60, before: 60 },
    indent: { left: 720 }
  });
}
function space() { return new Paragraph({ children: [new TextRun("")], spacing: { after: 80 } }); }

function tableRow(cells, header = false) {
  return new TableRow({
    tableHeader: header,
    children: cells.map((text, i) =>
      new TableCell({
        borders,
        margins: cellM,
        shading: header ? { fill: "1E3A5F", type: ShadingType.CLEAR } : (i % 2 === 0 ? { fill: "F5F8FF", type: ShadingType.CLEAR } : {}),
        children: [new Paragraph({
          children: [new TextRun({ text, bold: header, color: header ? "FFFFFF" : "000000", size: header ? 20 : 18 })]
        })]
      })
    )
  });
}

function makeTable(headers, rows, widths) {
  const total = widths.reduce((a, b) => a + b, 0);
  return new Table({
    width: { size: total, type: WidthType.DXA },
    columnWidths: widths,
    rows: [
      new TableRow({
        tableHeader: true,
        children: headers.map((text, i) =>
          new TableCell({
            borders,
            margins: cellM,
            width: { size: widths[i], type: WidthType.DXA },
            shading: { fill: "1E3A5F", type: ShadingType.CLEAR },
            children: [new Paragraph({ children: [new TextRun({ text, bold: true, color: "FFFFFF", size: 20 })] })]
          })
        )
      }),
      ...rows.map((row, ri) =>
        new TableRow({
          children: row.map((text, i) =>
            new TableCell({
              borders,
              margins: cellM,
              width: { size: widths[i], type: WidthType.DXA },
              shading: ri % 2 === 0
                ? { fill: "EEF3FF", type: ShadingType.CLEAR }
                : { fill: "FFFFFF", type: ShadingType.CLEAR },
              children: [new Paragraph({ children: [new TextRun({ text, size: 18 })] })]
            })
          )
        })
      )
    ]
  });
}

/* ====================================================================
   DOCUMENT
   ==================================================================== */
const doc = new Document({
  numbering: {
    config: [{
      reference: "bullets",
      levels: [{
        level: 0,
        format: LevelFormat.BULLET,
        text: "\u2022",
        alignment: AlignmentType.LEFT,
        style: { paragraph: { indent: { left: 720, hanging: 360 } } }
      }]
    }]
  },
  styles: {
    default: { document: { run: { font: "Calibri", size: 22 } } },
    paragraphStyles: [
      {
        id: "Heading1", name: "Heading 1", basedOn: "Normal", next: "Normal", quickFormat: true,
        run: { size: 36, bold: true, font: "Calibri", color: "1E3A5F" },
        paragraph: { spacing: { before: 360, after: 200 }, outlineLevel: 0, border: { bottom: { style: BorderStyle.SINGLE, size: 8, color: "2E75B6" } } }
      },
      {
        id: "Heading2", name: "Heading 2", basedOn: "Normal", next: "Normal", quickFormat: true,
        run: { size: 28, bold: true, font: "Calibri", color: "2E75B6" },
        paragraph: { spacing: { before: 280, after: 140 }, outlineLevel: 1 }
      },
      {
        id: "Heading3", name: "Heading 3", basedOn: "Normal", next: "Normal", quickFormat: true,
        run: { size: 24, bold: true, font: "Calibri", color: "1F4E79" },
        paragraph: { spacing: { before: 200, after: 100 }, outlineLevel: 2 }
      },
    ]
  },
  sections: [{
    properties: {
      page: {
        size: { width: 11906, height: 16838 },
        margin: { top: 1440, right: 1260, bottom: 1440, left: 1440 }
      }
    },
    headers: {
      default: new Header({
        children: [
          new Paragraph({
            children: [
              new TextRun({ text: "Rede de Email — Trabalho Final  |  Estrutura de Dados II  |  ISPTEC", size: 16, color: "888888", font: "Calibri" })
            ],
            border: { bottom: { style: BorderStyle.SINGLE, size: 4, color: "CCCCCC" } }
          })
        ]
      })
    },
    footers: {
      default: new Footer({
        children: [
          new Paragraph({
            children: [
              new TextRun({ text: "Página ", size: 16, color: "888888" }),
              new TextRun({ children: [PageNumber.CURRENT], size: 16, color: "888888" }),
              new TextRun({ text: " de ", size: 16, color: "888888" }),
              new TextRun({ children: [PageNumber.TOTAL_PAGES], size: 16, color: "888888" })
            ],
            alignment: AlignmentType.RIGHT,
            border: { top: { style: BorderStyle.SINGLE, size: 4, color: "CCCCCC" } }
          })
        ]
      })
    },
    children: [

      /* ==================== CAPA ==================== */
      space(), space(), space(),
      new Paragraph({
        children: [new TextRun({ text: "TRABALHO FINAL", font: "Calibri", size: 52, bold: true, color: "1E3A5F" })],
        alignment: AlignmentType.CENTER, spacing: { after: 200 }
      }),
      new Paragraph({
        children: [new TextRun({ text: "Rede de Email com Detecção de Spam", font: "Calibri", size: 36, color: "2E75B6" })],
        alignment: AlignmentType.CENTER, spacing: { after: 600 }
      }),
      new Paragraph({
        children: [new TextRun({ text: "Disciplina: Estrutura de Dados II", font: "Calibri", size: 24, color: "444444" })],
        alignment: AlignmentType.CENTER, spacing: { after: 100 }
      }),
      new Paragraph({
        children: [new TextRun({ text: "Instituição: ISPTEC — Instituto Superior Politécnico de Tecnologias e Ciências", font: "Calibri", size: 24, color: "444444" })],
        alignment: AlignmentType.CENTER, spacing: { after: 100 }
      }),
      new Paragraph({
        children: [new TextRun({ text: "Linguagem: C (C11)", font: "Calibri", size: 24, color: "444444" })],
        alignment: AlignmentType.CENTER, spacing: { after: 800 }
      }),
      new Paragraph({
        children: [new TextRun({ text: "Luanda, 2025", font: "Calibri", size: 22, color: "888888" })],
        alignment: AlignmentType.CENTER
      }),

      /* quebra de página */
      new Paragraph({ children: [new PageBreak()] }),

      /* ==================== 1. INTRODUÇÃO ==================== */
      h1("1. Introdução"),
      p("Este documento descreve o projecto de Trabalho Final da disciplina de Estrutura de Dados II. O objectivo é simular uma rede de comunicação por email entre um grupo de amigos, com suporte a detecção e filtragem automática de mensagens spam."),
      p("A rede é representada por um grafo não-dirigido, onde cada nó é um membro e cada aresta representa uma ligação directa entre dois membros. As mensagens só podem ser enviadas entre membros directamente ligados, e são automaticamente classificadas como spam ou normais com base num universo de palavras definido pelo administrador."),
      space(),

      /* ==================== 2. REQUISITOS ==================== */
      h1("2. Requisitos do Sistema"),
      h2("2.1 Requisitos Funcionais"),
      bullet("Gerir membros da rede (adicionar, listar)"),
      bullet("Criar e remover ligações directas entre membros"),
      bullet("Enviar mensagens apenas entre membros directamente ligados"),
      bullet("Classificar mensagens automaticamente como spam se contiverem k ou mais palavras do universo P"),
      bullet("Encaminhar spam para caixa de spam separada"),
      bullet("Impor limite de capacidade em ambas as caixas (inbox e spam_box)"),
      bullet("Permitir ao utilizador gerir o universo de palavras spam e o limiar k"),
      space(),
      h2("2.2 Requisitos Não-Funcionais"),
      bullet("Implementado em linguagem C (padrão C11)"),
      bullet("Estruturas de dados adequadas à natureza de cada problema"),
      bullet("Libertação correcta de memória alocada dinamicamente"),
      bullet("Código modular e comentado"),
      space(),

      /* ==================== 3. ARQUITECTURA ==================== */
      h1("3. Arquitectura do Sistema"),
      p("O projecto está organizado em módulos independentes, cada um responsável por uma camada lógica do sistema:"),
      space(),
      makeTable(
        ["Ficheiro", "Responsabilidade"],
        [
          ["include/rede.h", "Definição de todas as estruturas e protótipos — ponto único de inclusão"],
          ["src/rede.c",     "Operações sobre o grafo: adicionar membros, ligações, verificar adjacência"],
          ["src/spam.c",     "Gestão do universo de palavras spam e algoritmo de classificação"],
          ["src/fila.c",     "Fila duplamente encadeada (deque) para inbox e spam_box"],
          ["src/mensagem.c", "Lógica de envio e visualização das caixas de correio"],
          ["src/utils.c",    "Utilitários (tolower, menu interactivo) e dados de demonstração"],
          ["src/main.c",     "Ponto de entrada: inicialização, ciclo de menu, libertação de memória"],
          ["Makefile",       "Compilação automática com GCC, flags -Wall -Wextra -std=c11"],
        ],
        [3600, 5400]
      ),
      space(),

      /* ==================== 4. ESTRUTURAS DE DADOS ==================== */
      h1("4. Estruturas de Dados Utilizadas"),

      h2("4.1 Grafo — Lista de Adjacência"),
      p("A rede de membros é modelada como um grafo não-dirigido implementado com listas de adjacência. Esta escolha é justificada por:"),
      bullet("Redes sociais são tipicamente esparsas — poucos membros ligados a todos os outros"),
      bullet("Lista de adjacência tem complexidade O(V + E) em espaço, muito mais eficiente que matriz O(V²) para grafos esparsos"),
      bullet("Verificar se existe ligação (O(grau do nó)) é suficientemente rápido para o contexto do problema"),
      space(),
      p("Estrutura interna:", { bold: true }),
      code("typedef struct NoAdj {"),
      code("    int id_membro;      // id do membro vizinho"),
      code("    struct NoAdj *prox; // próximo nó da lista"),
      code("} NoAdj;"),
      code(""),
      code("typedef struct {"),
      code("    Membro  membros[MAX_MEMBROS]; // array estático de membros"),
      code("    NoAdj  *adj[MAX_MEMBROS];     // cabeças das listas de adjacência"),
      code("    int     num_membros;"),
      code("} Rede;"),
      space(),

      h2("4.2 Fila Duplamente Encadeada — Deque"),
      p("Cada membro possui duas caixas de correio (inbox e spam_box), ambas implementadas como filas duplamente encadeadas (deque). A política é FIFO: mensagens entram no fim e saem pelo início."),
      bullet("Inserção no fim: O(1) — mantemos ponteiro para o último elemento"),
      bullet("Remoção do início: O(1) — mantemos ponteiro para o primeiro elemento"),
      bullet("A estrutura duplamente ligada facilita uma futura extensão para remoção de mensagens específicas em O(1)"),
      bullet("Cada fila tem um campo limite — quando atingido, novas mensagens são recusadas (sem crash)"),
      space(),
      p("Estrutura interna:", { bold: true }),
      code("typedef struct Mensagem {"),
      code("    char       remetente[MAX_NOME];"),
      code("    char       conteudo[MAX_MENSAGEM];"),
      code("    int        is_spam;"),
      code("    struct Mensagem *ant;   // ponteiro para anterior"),
      code("    struct Mensagem *prox;  // ponteiro para seguinte"),
      code("} Mensagem;"),
      space(),

      h2("4.3 Lista Ligada Simples — Universo de Palavras Spam"),
      p("O universo de P palavras spam é mantido como uma lista ligada simples de cabeça. A escolha justifica-se pelo seguinte:"),
      bullet("O universo é tipicamente pequeno e sofre poucas modificações durante a execução"),
      bullet("A pesquisa é linear O(P), mas P é pequeno na prática"),
      bullet("A lista ligada permite crescimento dinâmico sem realocar memória (ao contrário de um array)"),
      bullet("Inserção no início é O(1)"),
      space(),
      p("Estrutura interna:", { bold: true }),
      code("typedef struct NoPalavra {"),
      code("    char palavra[50];"),
      code("    struct NoPalavra *prox;"),
      code("} NoPalavra;"),
      code(""),
      code("typedef struct {"),
      code("    NoPalavra *inicio;"),
      code("    int        total;  // P — total de palavras"),
      code("    int        k;      // limiar de classificação"),
      code("} ListaPalavrasSpam;"),
      space(),

      /* ==================== 5. ALGORITMOS ==================== */
      h1("5. Algoritmos Implementados"),

      h2("5.1 Detecção de Spam"),
      p("O algoritmo de detecção percorre o universo de palavras e verifica, para cada uma, se aparece no conteúdo da mensagem (usando strstr). A comparação é feita em lowercase para ser case-insensitive."),
      space(),
      p("Pseudocódigo:", { bold: true }),
      code("funcao e_spam(universo, mensagem, k):"),
      code("    msg_lower = toLower(mensagem)"),
      code("    contagem = 0"),
      code("    para cada palavra em universo:"),
      code("        se palavra_lower está em msg_lower:"),
      code("            contagem++"),
      code("    retorna contagem >= k"),
      space(),
      p("Complexidade: O(P × |mensagem|) onde P é o número de palavras no universo. Para P e mensagens pequenas, esta solução é prática e directa."),
      space(),

      h2("5.2 Envio de Mensagem"),
      p("O fluxo de envio segue estes passos:"),
      bullet("1. Validar IDs de remetente e destinatário"),
      bullet("2. Verificar existência de ligação directa (percorre lista de adjacência)"),
      bullet("3. Classificar a mensagem como spam ou normal"),
      bullet("4. Verificar se a caixa destino (inbox ou spam_box) tem capacidade"),
      bullet("5. Enfileirar a mensagem ou reportar que a caixa está cheia"),
      space(),

      /* ==================== 6. FLUXO DE DADOS ==================== */
      h1("6. Fluxo de Dados — Diagrama"),
      p("O diagrama seguinte ilustra o percurso de uma mensagem dentro do sistema:"),
      space(),
      makeTable(
        ["Etapa", "Acção", "Resultado possível"],
        [
          ["1", "Utilizador chama enviar_mensagem(orig, dest, texto)", "—"],
          ["2", "Valida IDs dos membros", "Erro se ID inválido"],
          ["3", "Verifica existe_ligacao(orig, dest)", "Erro se não há ligação directa"],
          ["4", "e_spam() conta palavras do universo na mensagem", "Spam = 1 se contagem >= k"],
          ["5a (spam)", "Verifica spam_box.total < spam_box.limite", "Descartada se cheia"],
          ["5b (normal)", "Verifica inbox.total < inbox.limite", "Descartada se cheia"],
          ["6", "enfileirar() insere mensagem no fim da fila correcta", "Mensagem entregue"],
        ],
        [600, 4200, 3000]
      ),
      space(),

      /* ==================== 7. COMO COMPILAR E EXECUTAR ==================== */
      h1("7. Como Compilar e Executar"),
      h2("7.1 Compilação"),
      p("Pré-requisitos: GCC e Make instalados no sistema."),
      code("# Dentro da pasta do projecto:"),
      code("make"),
      code(""),
      code("# Ou manualmente:"),
      code("gcc -Wall -Wextra -std=c11 -Iinclude -o rede_email \\"),
      code("    src/main.c src/rede.c src/spam.c \\"),
      code("    src/fila.c src/mensagem.c src/utils.c"),
      space(),
      h2("7.2 Execução"),
      code("./rede_email"),
      space(),
      p("Ao iniciar, o sistema carrega automaticamente 4 membros de demonstração (Alice, Bruno, Carlos, Diana) com ligações entre eles, e 5 palavras no universo spam com k=2."),
      space(),
      h2("7.3 Exemplo de Interacção"),
      makeTable(
        ["Acção", "Mensagem enviada", "Resultado"],
        [
          ["Alice → Bruno", "\"Clique aqui para ganhar gratis\"", "SPAM (3 palavras: clique, ganhe, gratis >= k=2)"],
          ["Alice → Carlos", "\"Bom dia, como estás?\"", "INBOX (0 palavras spam < k=2)"],
          ["Bruno → Carlos", "Qualquer mensagem", "ERRO (não há ligação directa)"],
        ],
        [2000, 3800, 3000]
      ),
      space(),

      /* ==================== 8. SOLUÇÕES ALTERNATIVAS ==================== */
      h1("8. Soluções Alternativas e Justificação das Escolhas"),

      h2("8.1 Representação do Grafo: Matriz de Adjacência"),
      p("Uma matriz booleana NxN indicaria se existe ligação entre cada par de membros."),
      makeTable(
        ["Critério", "Lista de Adjacência (escolhida)", "Matriz de Adjacência"],
        [
          ["Espaço", "O(V + E) — eficiente para grafos esparsos", "O(V²) — desperdiça memória se poucas ligações"],
          ["Verificar ligação", "O(grau) — proporcional ao número de vizinhos", "O(1) — acesso directo à célula"],
          ["Listar vizinhos", "O(grau) — percorre só os vizinhos reais", "O(V) — percorre toda a linha"],
          ["Adequação", "Ideal para redes sociais esparsas", "Ideal para grafos densos ou muitas consultas pontuais"],
        ],
        [2000, 3000, 3800]
      ),
      p("Conclusão: para uma rede social típica, a lista de adjacência é mais eficiente em memória. A matriz seria preferível se precisássemos de muitas consultas de ligação isoladas em tempo O(1)."),
      space(),

      h2("8.2 Fila de Mensagens: Array Circular"),
      p("Uma fila implementada sobre um array circular de tamanho fixo seria uma alternativa:"),
      bullet("Vantagem: acesso por índice O(1), sem overhead de ponteiros"),
      bullet("Vantagem: sem alocação dinâmica — mais simples e menos propenso a memory leaks"),
      bullet("Desvantagem: tamanho máximo fixo em tempo de compilação"),
      bullet("Desvantagem: remoção de mensagens específicas no meio seria O(N)"),
      p("A lista duplamente encadeada foi preferida pela flexibilidade de crescimento e pela facilidade de extensão futura (ex: remover mensagem específica sem reordenar)."),
      space(),

      h2("8.3 Detecção de Spam: Outras Abordagens"),
      makeTable(
        ["Abordagem", "Descrição", "Complexidade", "Quando usar"],
        [
          ["strstr linear (escolhida)", "Percorre lista de palavras e procura cada uma na mensagem", "O(P × |msg|)", "P pequeno, simples de implementar"],
          ["Hash Set", "Tokeniza a mensagem e verifica cada token numa tabela hash", "O(|msg|) amortizado", "P grande, palavras exactas"],
          ["Árvore Trie", "Trie de palavras spam; pesquisa simultânea de múltiplas palavras", "O(|msg|)", "P muito grande, prefixos comuns"],
          ["Naive Bayes", "Modelo probabilístico treinado com exemplos de spam/não-spam", "O(tokens)", "Produção, alta precisão"],
        ],
        [2000, 2800, 1800, 2200]
      ),
      p("Para o contexto académico com P pequeno, a abordagem linear com strstr é clara, correcta e suficiente. Em produção, uma Hash Set ou Trie seriam preferíveis."),
      space(),

      h2("8.4 Universo Spam: Array vs Lista Ligada"),
      bullet("Array estático: mais simples, acesso por índice, mas tamanho fixo"),
      bullet("Lista ligada (escolhida): crescimento dinâmico, inserção O(1), sem limite de palavras"),
      bullet("Hash Table: pesquisa O(1) para palavras exactas — preferível para P muito grande"),
      space(),

      /* ==================== 9. LIMITAÇÕES E EXTENSÕES ==================== */
      h1("9. Limitações e Possíveis Extensões"),
      h2("9.1 Limitações Actuais"),
      bullet("Persistência: os dados perdem-se quando o programa termina (não há ficheiro de estado)"),
      bullet("Remoção de membros não implementada (requereria actualizar todas as listas de adjacência)"),
      bullet("Detecção de spam por substring pode gerar falsos positivos (ex: \"gratuito\" activa \"gratis\" via strstr)"),
      bullet("Sem suporte a envio por difusão (broadcast) — só mensagens ponto-a-ponto"),
      space(),
      h2("9.2 Extensões Sugeridas"),
      bullet("Persistência com ficheiros de texto ou binários (fopen/fwrite)"),
      bullet("Detecção de spam por tokenização de palavras completas (evita falsos positivos)"),
      bullet("BFS/DFS para descobrir se existe caminho entre dois membros não directamente ligados"),
      bullet("Encaminhamento de mensagens por caminhos (routing) — mensagem passa por intermediários"),
      bullet("Hash Table para o universo spam, melhorando a detecção para O(tokens_mensagem)"),
      bullet("Interface com ficheiro de configuração para carregar rede e universo spam automaticamente"),
      space(),

      /* ==================== 10. CONCLUSÃO ==================== */
      h1("10. Conclusão"),
      p("O projecto implementa com sucesso todos os requisitos definidos no enunciado: rede de membros com grafo de adjacência, caixas de mensagens com limite, detecção e filtragem automática de spam, e interface interactiva."),
      p("As estruturas de dados escolhidas — lista de adjacência, fila duplamente encadeada e lista ligada simples — são adequadas para o problema proposto, com boa relação entre simplicidade de implementação e eficiência. A modularidade do código facilita a manutenção e extensão futura."),
      p("As alternativas discutidas mostram que o projecto está consciente das trocas inerentes a cada estrutura, e que as escolhas foram feitas com justificação técnica sólida."),
      space(),

    ]
  }]
});

Packer.toBuffer(doc).then(buf => {
  fs.writeFileSync("/mnt/user-data/outputs/Relatorio_Rede_Email.docx", buf);
  console.log("Documento gerado com sucesso.");
});
