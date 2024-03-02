CXX := g++
CXX_FLAGS := -std=c++11 -g -O0

BIN := bin
SRC := src
INCLUDE := include
EIGEN := ${HOME}/lib/eigen-3.4.0
BOOST := ${HOME}/lib/boost_1_78_0

LIBRARIES :=  ${HOME}/lib/boost_1_78_0/stage/lib
SHARED_LIB := -lboost_program_options
EXECUTABLE := mcmc

all : $(BIN)/$(EXECUTABLE)

run : clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) -I $(INCLUDE) -I $(EIGEN) -I $(BOOST) $^ -o $@ \
	-Wl,-rpath=$(LIBRARIES) -L $(LIBRARIES) $(SHARED_LIB)

clean:
	-rm $(BIN)/*