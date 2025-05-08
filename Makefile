CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++17 -O0

TARGET = egzek

SRCS = main.cpp ./struktury/heap.cpp
HEADERS = ./struktury/hash.hh ./struktury/dynamic_array.hh ./struktury/list.hh ./struktury/heap.hh ./struktury/pair.hh testy.hh wczytaj.hh

all: $(TARGET)

$(TARGET): $(SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)
