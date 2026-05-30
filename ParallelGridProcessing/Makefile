CXX=mpicxx
CXXFLAGS=-O2 -std=c++17

SRC_DIR=src
BIN_MAIN=mpi_app
BIN_GEN=generate_matrix

SRCS=$(SRC_DIR)/main.cpp $(SRC_DIR)/heat_diffusion.cpp $(SRC_DIR)/matmul.cpp $(SRC_DIR)/matrix_io.cpp

all: $(BIN_MAIN) $(BIN_GEN)

$(BIN_MAIN): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $(SRCS)

$(BIN_GEN): $(SRC_DIR)/generate_matrix.cpp $(SRC_DIR)/matrix_io.cpp
	$(CXX) $(CXXFLAGS) -o $@ $(SRC_DIR)/generate_matrix.cpp $(SRC_DIR)/matrix_io.cpp

clean:
	rm -f $(BIN_MAIN) $(BIN_GEN)
