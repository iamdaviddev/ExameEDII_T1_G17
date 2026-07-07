# 📧 Rede de Comunicação por Email com Detecção de Spam

[![C Standard](https://img.shields.io/badge/C-C11-blue.svg)](https://en.wikipedia.org/wiki/C11_(C_standard_revision))
[![Academic Project](https://img.shields.io/badge/Academic-ISPTEC-gold.svg)]
[![License](https://img.shields.io/badge/License-MIT-green.svg)](https://opensource.org/licenses/MIT)

Este projeto consiste na **simulação de uma rede de comunicação por email entre um grupo de amigos**, desenvolvida inteiramente em **Linguagem C (padrão C11)**. O sistema modela os membros como um grafo não-dirigido, aplica limites de capacidade nas caixas de correio e conta com um algoritmo integrado para classificação e filtragem automática de mensagens do tipo *Spam*.

Trabalho Final apresentado à disciplina de **Estrutura de Dados II** do curso de **Engenharia Informática** no **Instituto Superior Politécnico de Tecnologias e Ciências (ISPTEC)**.

---

## 🛠️ Arquitetura do Sistema & Estruturas de Dados

O projeto está organizado numa estrutura estritamente modular, dividindo as responsabilidades funcionais em 7 ficheiros distintos:

| Ficheiro | Responsabilidade |
| :--- | :--- |
| `include/rede.h` | Definição de todas as estruturas e protótipos (ponto único de inclusão). |
| `src/rede.c` | Operações sobre o grafo: adicionar membros, ligações e verificar adjacência. |
| `src/spam.c` | Gestão do universo de palavras spam e algoritmo de classificação. |
| `src/fila.c` | Fila duplamente encadeada (deque) para as caixas `inbox` e `spam_box`. |
| `src/mensagem.c` | Lógica de envio de mensagens e visualização das caixas de correio. |
| `src/utils.c` | Menu interativo, utilitários e dados de demonstração pré-carregados. |
| `src/main.c` | Ponto de entrada: inicialização, ciclo de menu e libertação de memória. |

### Estruturas de Dados Adotadas

* **Rede de Membros (Grafo Não-Dirigido):** Modelada através de **Listas de Adjacência**. Esta escolha justifica-se pela natureza esparsa da rede, garantindo uma complexidade espacial de $O(V + E)$.
* **Caixas de Correio (`inbox` e `spam_box`):** Implementadas como uma **Fila Duplamente Encadeada (Deque)** seguindo a política FIFO. Garante inserção no fim e remoção no início em tempo constante $O(1)$.
* **Universo Spam:** Mantido numa **Lista Ligada Simples** com inserção no início em tempo $O(1)$, ideal para o crescimento dinâmico de um universo tipicamente pequeno.

---

## 🔬 Algoritmos e Fluxo de Envio

### 1. Detecção de Spam
O algoritmo percorre o universo de palavras e verifica, de forma *case-insensitive* (usando `strstr` após converter para minúsculas), se elas aparecem no conteúdo da mensagem. Uma mensagem é classificada como spam se a contagem de correspondências for maior ou igual ao limiar $k$ definido.
* **Complexidade:** $O(P \times |mensagem|)$, onde $P$ é o número de palavras no universo spam.

### 2. Passo a Passo do Envio
1. **Validação:** Verifica os IDs de remetente e destinatário.
2. **Conexão:** Garante que existe uma ligação direta (aresta) no grafo entre os dois membros.
3. **Classificação:** O algoritmo avalia se a mensagem é ou não spam com base no limiar $k$.
4. **Capacidade:** Valida se a caixa de destino (`inbox` ou `spam_box`) tem espaço disponível antes de realizar o enfileiramento.

---

## 🚀 Como Compilar e Executar

O projeto utiliza o GCC com suporte ao padrão C11 e as flags de aviso `-Wall -Wextra`. A compilação está automatizada via **Makefile**.

### Compilação Automatizada
Na raiz do diretório do projeto, executa:
```bash
make