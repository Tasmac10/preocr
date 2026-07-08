# Remoção de ruído (Parte 1)

**Membro 2: Tássio Mateus de Carvalho**

Módulo: `include/noise.hpp`, `src/noise.cpp`. Teste: `tests/test_noise.cpp`.

## 1. O que foi feito

O módulo recebe uma imagem binária (PBM P1) e devolve uma nova imagem com o ruído
sal-e-pimenta removido, preservando o texto. A função pública é:

```cpp
Image remove_noise(const Image& img);
```

Ela é uma função pura (recebe a imagem original por referência constante e devolve
uma imagem nova), sem nenhuma operação de arquivo, o que a mantém independente do
módulo de entrada/saída (Membro 1) e facilita o teste isolado.

A solução final combina duas etapas:

1. **Remoção de pimenta** por análise de componentes conexos: apaga todo objeto
   preto isolado com tamanho menor ou igual a um limiar (2 pixels).
2. **Preenchimento de sal**: preenche buracos brancos de 1 pixel cercados pelos 8
   vizinhos pretos.

## 2. Técnicas aplicadas e parâmetros

| Técnica | Parâmetro | Justificativa |
|---------|-----------|---------------|
| Vizinhança 3×3 | janela de 8 vizinhos | o ruído é de 1 pixel; janela maior erodiria letras de altura mínima 12 px |
| Componentes conexos (flood fill) | conectividade **8** | trata blobs de ruído horizontais, verticais e diagonais como um único objeto |
| Filtro por tamanho de componente | limiar **≤ 2 px** | remove blobs de 1 e 2 pixels; letras são muito maiores e ficam intactas |
| Preenchimento de sal | exatamente **8** vizinhos pretos | só preenche buraco totalmente cercado, evitando alterar bordas legítimas |

Detalhes de implementação do flood fill:

- **Pilha explícita** (busca em profundidade iterativa) em vez de recursão, porque
  a imagem tem cerca de 3,8 milhões de pixels e um flood fill recursivo estouraria
  a pilha de chamadas do programa.
- **Marcação de visitado no momento de empilhar**, garantindo que cada pixel entra
  na fronteira no máximo uma vez.
- **Índice único** (`linha * largura + coluna`) para representar cada pixel, em vez
  de um par de coordenadas.
- Leitura sempre da imagem **original** e escrita numa imagem de **saída separada**,
  para que uma correção não contamine a decisão sobre os pixels vizinhos.

## 3. Principais problemas encontrados e soluções

### 3.1 A mediana clássica erode o texto

A primeira tentativa foi um filtro de mediana 3×3 (voto de maioria, `≥ 5` de 9).
Como as letras da Arial nessa resolução têm traços de 1 a 2 pixels de largura, um
pixel no meio de um traço fino tem poucos vizinhos pretos e "perde a votação",
sendo apagado. Medindo o dano na imagem limpa (onde qualquer alteração é dano),
a mediana modificava **49864 pixels** de texto legítimo. Descartada.

### 3.2 Solução conservadora: remover apenas pixels isolados

Trocamos a maioria por uma regra cirúrgica: remove um pixel preto apenas se ele
tiver **0 vizinhos pretos** (pimenta isolada). Um pixel de traço sempre tem pelo
menos 1 vizinho preto, então nunca é removido. Resultado: **0 de dano** na imagem
limpa, e cerca de 92,6% do ruído removido na imagem com ruído.

### 3.3 Ruído de 2 pixels adjacentes

Inspecionando as imagens de diferença, notamos blobs de pimenta com 2 pixels
grudados. A regra de 0 vizinhos não os pega (cada pixel vê o outro como vizinho).
A tentativa de afrouxar para `≤ 1 vizinho` removia mais ruído, mas reintroduzia
dano (**68 pixels**), porque a ponta de um traço também tem 1 vizinho preto,
sendo indistinguível de um blob de 2 pixels apenas pela vizinhança local.

A solução correta foi olhar o **objeto inteiro** com componentes conexos: um blob
de 2 pixels é um componente de tamanho 2 e é removido; a ponta de uma letra
pertence ao componente da letra inteira, que é grande, e é preservada. Isso removeu
os blobs de 2 pixels mantendo **0 de dano**.

### 3.4 Ruído colado nas letras (não resolvido, decisão consciente)

Sobra um ruído residual grudado nas bordas das letras. Por estar conectado ao
texto, ele vira parte do componente grande da letra e o filtro por tamanho não o
alcança. Consideramos tratá-lo com morfologia matemática (abertura e fechamento
com elemento estruturante 3×3), mas estimamos que a abordagem não seria viável
nessa resolução. Os traços da fonte têm apenas 1 a 2 pixels de largura, mais finos
que o menor elemento estruturante possível, de modo que a erosão destruiria boa
parte do texto legítimo. O dano ao conteúdo superaria em muito o ruído removido, e
o mesmo raciocínio inviabiliza a abertura por reconstrução (não restaria núcleo do
traço para reconstruir). Por isso a técnica foi descartada sem implementação.

Como esse ruído residual não altera a contagem de linhas/palavras nem os
retângulos delimitadores de forma relevante (está dentro das letras), optamos por
**preservar o texto e não removê-lo**, em vez de arriscar destruir o conteúdo. Um
tratamento por reconhecimento de caractere (template matching) resolveria, mas
exige reconhecer a letra antes de limpá-la, o que é circular e pertence à etapa de
OCR (bônus), fora do escopo deste módulo.

## 4. Resultados

Redução de ruído por imagem de teste (diferença de pixels em relação à versão
limpa, antes e depois do filtro):

| Imagem | Ruído inicial | Ruído restante | Redução |
|--------|--------------:|---------------:|--------:|
| c02          | 17518 | 1184 | 93,24% |
| c02_espacos  | 18716 |  957 | 94,89% |
| c03          | 63473 | 7284 | 88,52% |
| c02_just     | 63279 | 7442 | 88,24% |
| c03_just     | 63756 | 7331 | 88,50% |

**Dano em imagem limpa: 0 pixels** em todos os casos testados. O filtro nunca
altera texto legítimo.

## 5. Eficiência

O flood fill visita cada pixel exatamente uma vez ao longo de toda a execução
(controlado pela matriz de visitados), então a etapa de pimenta é O(número de
pixels). O preenchimento de sal é uma passada 3×3 por pixel, também linear. O
custo total é linear no tamanho da imagem, com constante pequena.

## 6. Metodologia de validação

Como o módulo de leitura oficial (Membro 1) ainda não estava pronto, foi criado um
programa de teste independente (`tests/test_noise.cpp`) com um leitor/gravador PBM
temporário e seu próprio `main`. Ele:

- lê uma imagem com ruído, aplica `remove_noise` e grava a saída;
- opcionalmente recebe a imagem limpa de referência e reporta o número de pixels de
  diferença antes e depois, além do percentual de redução;
- gera uma imagem de diferença (`_diff.pbm`) destacando os pixels que ainda diferem
  da referência, para inspeção visual do ruído restante.

O par de imagens limpa/com-ruído da base de teste permitiu medir objetivamente
tanto a **qualidade** (redução do ruído) quanto o **dano** (rodando o filtro sobre
a própria imagem limpa: qualquer diferença é dano).

## 7. Reprodução dos testes

```bash
make test
# qualidade e imagem de diferença:
./test_noise images/samples/lorem_s12_c02_noise.pbm out.pbm images/samples/lorem_s12_c02.pbm
# dano (imagem limpa nos dois argumentos): o "depois" deve ser 0
./test_noise images/samples/lorem_s12_c02.pbm out.pbm images/samples/lorem_s12_c02.pbm
```

## 8. Figuras sugeridas para o relatório final

- Recorte da imagem original com ruído sal-e-pimenta.
- A mesma região após o filtro.
- Imagem de diferença (`_diff.pbm`) mostrando o ruído residual concentrado nas
  bordas das letras.
