# Divisão de Tarefas — PreOCR

Divisão do trabalho em 4 fatias verticais equilibradas. Cada membro fica com um
bloco algorítmico central do pipeline e todos contribuem igualmente com imagens
de teste e com sua seção do relatório. As tarefas cobrem os critérios de avaliação.

A divisão é conceitual e independe da linguagem escolhida (Python, Java, C++ ou C).

## Membro 1 — Entrada/Saída, CLI e núcleo de dados

*Base do projeto; todos os demais dependem desta entrega.*

- Estrutura de dados da imagem binária (matriz + dimensões)
- Leitura de PBM P1 ASCII (parser robusto: comentários `#`, espaços, dimensões)
- Escrita de PBM P1
- Parsing dos argumentos de linha de comando
- **Tratamento de erros** (arquivo inexistente, formato inválido, dimensões erradas) → critério *1.0*
- Coordena a estrutura do repositório / build
- 1 imagem de teste + seção do relatório

## Membro 2 — Remoção de ruído (Parte 1)

- Filtro para ruído sal-e-pimenta (mediana/maioria em vizinhança, **implementado pelo grupo**)
- Ajuste e justificativa dos parâmetros (tamanho da janela)
- Salvar a imagem filtrada em novo PBM
- Foco em **eficiência** do filtro → contribui ao critério *2.0*
- 1 imagem de teste + seção do relatório

## Membro 3 — Segmentação de colunas e linhas

- Detecção de **colunas** (projeção vertical / gaps entre blocos)
- Detecção de **linhas** dentro de cada coluna (projeção horizontal)
- Contagem de colunas e linhas (linhas em colunas diferentes contam separadamente)
- 1 imagem de teste (com múltiplas colunas) + seção do relatório

## Membro 4 — Detecção de palavras e desenho da saída

- Detecção de **palavras** dentro das linhas (espaçamento horizontal / componentes conexos)
- Contagem de palavras
- Cálculo dos **retângulos delimitadores** (bounding boxes)
- Desenho dos retângulos e geração da imagem PBM de saída
- 1 imagem de teste + seção do relatório

## Equilíbrio e dependências

| Membro | Bloco central | Peso |
|--------|---------------|------|
| 1 | I/O + CLI + erros | infraestrutura crítica |
| 2 | Filtro de ruído | 1 algoritmo |
| 3 | Colunas + linhas | 1 algoritmo (2 etapas) |
| 4 | Palavras + retângulos | 1 algoritmo + saída |

- **Ordem:** o Membro 1 entrega primeiro a estrutura de dados + I/O (contrato de
  interface). Depois os Membros 2, 3 e 4 trabalham em paralelo, cada um
  consumindo/produzindo a matriz binária. Integração ao final.
- **Compartilhado por todos:** 1 imagem de teste cada (4 no total, cobrindo os 3
  exigidos + 1 extra) e a seção correspondente do relatório.
- **Extra (bônus até 2.0):** distribuir por interesse. Ex.: reconhecimento de
  caracteres → Membro 4; fontes/tamanhos variados → Membro 3.
