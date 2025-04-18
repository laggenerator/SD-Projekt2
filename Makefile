CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++17 -O0

TARGET = egzek

SRCS = main.cpp dynamic_array.cpp list.cpp heap.cpp
HEADERS = dynamic_array.hh list.hh heap.hh pair.hh prique.hh generator.hh

all: $(TARGET)

$(TARGET): $(SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)