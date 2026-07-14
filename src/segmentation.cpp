#include "segmentation.hpp"
#include <vector>

namespace preocr {

// [Membro 3] Segmenta a imagem em colunas e, dentro de cada coluna, em linhas.
// Colunas: projecao/gaps verticais. Linhas: projecao horizontal por coluna.
std::vector<Column> segment(const Image &img) {
    if (img.width <= 0 || img.height <= 0) {
        return {};
    }

    // 1. Projecao vertical global para encontrar as colunas
    std::vector<int> v_proj(img.width, 0);
    for (int y = 0; y < img.height; ++y) {
        for (int x = 0; x < img.width; ++x) {
            if (img.at(x, y) == 1) {
                v_proj[x]++;
            }
        }
    }

    std::vector<Column> colunas;
    bool in_col = false;
    int col_start = -1;
    int col_end = -1;
    int white_count = 0;
    const int MIN_COL_GAP = 20; // limite de pixels brancos para separar colunas

    for (int x = 0; x < img.width; ++x) {
        if (v_proj[x] > 0) {
            if (!in_col) {
                in_col = true;
                col_start = x;
            }
            col_end = x;
            white_count = 0;
        } else {
            if (in_col) {
                white_count++;
                if (white_count >= MIN_COL_GAP) {
                    Column col;
                    col.box.x0 = col_start;
                    col.box.x1 = col_end;
                    colunas.push_back(col);
                    in_col = false;
                }
            }
        }
    }
    if (in_col) {
        Column col;
        col.box.x0 = col_start;
        col.box.x1 = col_end;
        colunas.push_back(col);
    }

    // 2. Para cada coluna encontrada, identificar seus limites verticais e suas linhas
    std::vector<Column> colunas_validas;
    for (auto& col : colunas) {
        // Projecao horizontal dentro da coluna
        std::vector<int> h_proj(img.height, 0);
        for (int y = 0; y < img.height; ++y) {
            for (int x = col.box.x0; x <= col.box.x1; ++x) {
                if (img.at(x, y) == 1) {
                    h_proj[y]++;
                }
            }
        }

        // Encontrar limites y0 e y1 da coluna inteira
        int col_y0 = -1;
        int col_y1 = -1;
        for (int y = 0; y < img.height; ++y) {
            if (h_proj[y] > 0) {
                if (col_y0 == -1) {
                    col_y0 = y;
                }
                col_y1 = y;
            }
        }

        if (col_y0 == -1) {
            continue; // coluna vazia
        }

        col.box.y0 = col_y0;
        col.box.y1 = col_y1;

        // Identificar as linhas da coluna
        std::vector<Line> lines;
        bool in_line = false;
        int line_start = -1;
        int line_end = -1;
        int line_white_count = 0;
        const int MIN_LINE_GAP = 4; // limite de pixels brancos para separar linhas

        for (int y = col_y0; y <= col_y1; ++y) {
            if (h_proj[y] > 0) {
                if (!in_line) {
                    in_line = true;
                    line_start = y;
                }
                line_end = y;
                line_white_count = 0;
            } else {
                if (in_line) {
                    line_white_count++;
                    if (line_white_count >= MIN_LINE_GAP) {
                        Line line;
                        line.box.y0 = line_start;
                        line.box.y1 = line_end;
                        // Determina o x0 e x1 precisos para esta linha
                        int lx0 = col.box.x1;
                        int lx1 = col.box.x0;
                        for (int ly = line_start; ly <= line_end; ++ly) {
                            for (int lx = col.box.x0; lx <= col.box.x1; ++lx) {
                                if (img.at(lx, ly) == 1) {
                                    if (lx < lx0) lx0 = lx;
                                    if (lx > lx1) lx1 = lx;
                                }
                            }
                        }
                        line.box.x0 = lx0;
                        line.box.x1 = lx1;
                        lines.push_back(line);
                        
                        in_line = false;
                    }
                }
            }
        }
        if (in_line) {
            Line line;
            line.box.y0 = line_start;
            line.box.y1 = line_end;
            int lx0 = col.box.x1;
            int lx1 = col.box.x0;
            for (int ly = line_start; ly <= line_end; ++ly) {
                for (int lx = col.box.x0; lx <= col.box.x1; ++lx) {
                    if (img.at(lx, ly) == 1) {
                        if (lx < lx0) lx0 = lx;
                        if (lx > lx1) lx1 = lx;
                    }
                }
            }
            line.box.x0 = lx0;
            line.box.x1 = lx1;
            lines.push_back(line);
        }

        col.lines = lines;
        colunas_validas.push_back(col);
    }

    return colunas_validas;
}

} // namespace preocr
