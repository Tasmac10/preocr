# Makefile do PreOCR
# Uso:
#   make          -> compila o binario ./preocr
#   make run ARGS="images/samples/lorem_s12_c02.pbm saida.pbm"
#   make clean    -> remove objetos e binario

CXX      := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -Iinclude
BIN      := preocr
TEST_BIN := test_noise

SRC := $(wildcard src/*.cpp)
OBJ := $(SRC:.cpp=.o)

all: $(BIN)

$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(BIN)
	./$(BIN) $(ARGS)

# Teste isolado do modulo de ruido (Membro 2), independente do resto do pipeline.
# Ex.: make test && ./test_noise images/samples/lorem_s12_c02_noise.pbm out.pbm \
#      images/samples/lorem_s12_c02.pbm
test: $(TEST_BIN)

$(TEST_BIN): tests/test_noise.cpp src/noise.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(OBJ) $(BIN) $(TEST_BIN)

.PHONY: all run test clean
