SRC       = $(wildcard *.cpp)
OBJS      = $(SRC:%.cpp=%.o)
CXX       = gcc
DEBUG     = -g
CXXFLAGS  = -Wall $(DEBUG) -pthread -fpermissive
LDFLAGS   = -pthread
BIN       = run

all: $(OBJS)
	$(CXX) $(LDFLAGS) -o $(BIN) $^ $(LIBS)

clean:
	rm -rf *.o
	rm -rf $(BIN)
