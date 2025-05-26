CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++17 -O0 -g

TARGET = egzek

SRCS = main.cpp ./struktury/heap.cpp
HEADERS = ./struktury/IHash.hh ./struktury/cuckoo_hash.hh ./struktury/link_hash.hh ./struktury/linear_hash.hh ./struktury/hash_table.hh ./struktury/dynamic_array.hh ./struktury/list.hh ./struktury/heap.hh ./struktury/pair.hh testy.hh wczytaj.hh

all: $(TARGET)

$(TARGET): $(SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)
