# Makefile do PreOCR
# Uso:
#   make          -> compila o binario ./preocr
#   make run ARGS="images/samples/lorem_s12_c02.pbm saida.pbm"
#   make clean    -> remove objetos e binario

CXX      := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -Iinclude
BIN      := preocr

SRC := $(wildcard src/*.cpp)
OBJ := $(SRC:.cpp=.o)

all: $(BIN)

$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(BIN)
	./$(BIN) $(ARGS)

clean:
	rm -f $(OBJ) $(BIN)

.PHONY: all run clean
