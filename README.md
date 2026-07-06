# PreOCR

Trabalho prático da disciplina **Processamento de Imagens (T01 2026.1)** — Departamento de Computação, Universidade Federal de Sergipe.
Professora: Beatriz Trinchão Andrade.

## Objetivo

Programa que recebe, via linha de comando, uma imagem binária no formato **PBM ASCII (P1)** contendo um texto com uma ou mais colunas e:

1. Remove ruído sal-e-pimenta da imagem.
2. Conta e imprime no terminal o número de **colunas**, **linhas** e **palavras** do texto (linhas em colunas diferentes são contadas separadamente).
3. Gera uma imagem PBM de saída com cada palavra circunscrita por um retângulo.

De forma genérica, é o primeiro passo de um pipeline de OCR: uma vez definidas as áreas com palavras, o passo seguinte seria isolar e reconhecer as letras.

### Premissas da entrada

- Texto todo em fonte **Arial**, mesmo tamanho de fonte em todo o texto.
- Altura mínima de uma letra: **12 pixels**.
- A imagem não contém figuras, mas pode conter espaços vazios e ruído sal-e-pimenta (de 1 pixel).

## Estrutura do repositório

```
.
├── src/              # Código-fonte (funções implementadas pelo grupo)
├── include/          # Cabeçalhos / módulos auxiliares
├── images/
│   ├── input/        # Imagens de teste de entrada (.pbm)
│   └── output/       # Imagens geradas (filtradas / com retângulos)
├── report/           # Relatório do trabalho
├── docs/             # Especificação e material de apoio
└── README.md
```

## Etapas de desenvolvimento

O trabalho é dividido em duas partes:

- **Parte 1** — Abrir imagem PBM, remover ruído e salvar a imagem filtrada em um novo PBM. Criar três imagens de teste de autoria do grupo.
- **Parte 2** — Contagem de colunas, linhas e palavras, detecção das palavras (circunscrição com retângulo) e possíveis recursos extras.

### Nomenclatura das imagens de teste

```
grupo_I_imagem_X_linhas_Y_palavras_Z.pbm
```

- `I` — número do grupo (conferir no SIGAA)
- `X` — índice da imagem do grupo (1, 2 ou 3)
- `Y` — número de linhas do texto
- `Z` — número de palavras do texto

Exemplo (grupo 9): `grupo_09_imagem_2_linhas_25_palavras_259.pbm`

## Compilação e execução

> A linguagem ainda será definida (aceitas: Python, Java, C++, C). Esta seção será preenchida com as instruções de compilação e execução no Linux.

```
# exemplo (a definir)
# ./preocr images/input/grupo_XX_imagem_1_linhas_Y_palavras_Z.pbm images/output/saida.pbm
```

## Avaliação

| Critério | Pontos |
|----------|--------|
| Uso correto dos conceitos e atendimento às especificações | 5.0 |
| Eficiência | 2.0 |
| Organização do código-fonte | 0.5 |
| Tratamento de erros | 1.0 |
| Relatório | 1.5 |
| Extra (fontes variadas, reconhecimento de caracteres, etc.) | até 2.0 |

## Observações importantes

- As funções usadas **devem ser implementadas pelo grupo**.
- Trabalho em grupos de até quatro pessoas.
- Projeto e relatório enviados pelo SIGAA. Datas de entrega: vide SIGAA.
- Penalidade por atraso: 3 pontos por dia.
