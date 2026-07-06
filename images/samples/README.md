# Imagens de apoio (base fornecida)

Base de teste fornecida na disciplina. **Não são** as imagens próprias do grupo
(essas seguem a convenção `grupo_I_imagem_X_...` e ficam em `images/input/`).

## Convenção de nomes

`lorem_s12_c02_espacos_noise.pbm`

- `lorem` — texto (Lorem Ipsum)
- `s12` — fonte tamanho 12
- `c02` / `c03` — número de colunas (2 ou 3)
- `_just` — texto justificado
- `_espacos` — contém espaços vazios
- `_noise` — contém ruído sal-e-pimenta

Cada `.pbm` está em formato **PBM ASCII (P1)**. Os arquivos `.odt` e `.pdf` são os
documentos-fonte que geraram as imagens (referência de "verdade" para conferir
colunas, linhas e palavras).

## Arquivos

| Base | Limpa | Com ruído | Colunas |
|------|-------|-----------|---------|
| `lorem_s12_c02` | ✅ | `_noise` | 2 |
| `lorem_s12_c02_espacos` | ✅ | `_espacos_noise` | 2 |
| `lorem_s12_c02_just` | ✅ | `_just_noise` | 2 |
| `lorem_s12_c03` | ✅ | `_noise` | 3 |
| `lorem_s12_c03_just` | ✅ | `_just_noise` | 3 |
