#ifndef DYN_ARR_H
#define DYN_ARR_H

#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <cstring>
#include <cstdlib>

template<typename T>
class DynamicArray{
private:
  T* dane;
  std::size_t rozmiar;
  std::size_t pojemnosc;
    
  void shrink();
    
public:
  DynamicArray();
  ~DynamicArray();
  DynamicArray(const DynamicArray& inny);
    
  void push_back(T wartosc);
  void push_front(T wartosc);
  void push_at(unsigned int n, T wartosc);
    
  T remove_back();
  T remove_front();
  T remove_at(unsigned int n);
    
  int find(T wartosc);
  T at_position(unsigned int n);
  
  size_t get_size() const { return rozmiar; }
  size_t capacity() const { return pojemnosc; }
    
  void resize(std::size_t nowa_pojemnosc);
    
  void _show() const;
  T& operator[](size_t i); 
  DynamicArray& operator=(const DynamicArray& inny);

};

template<typename T>
DynamicArray<T>::DynamicArray() : dane(nullptr), rozmiar(0), pojemnosc(0) {}

template<typename T>
DynamicArray<T>::~DynamicArray() {
  if(dane) {
    delete[] dane;
  }
}

template<typename T>
DynamicArray<T>::DynamicArray(const DynamicArray& inny) {
  pojemnosc = inny.pojemnosc;
  rozmiar = inny.rozmiar;
  dane = new T[pojemnosc];
  for (std::size_t i = 0; i < rozmiar; ++i) {
    dane[i] = inny.dane[i];
  }
}

template<typename T>
void DynamicArray<T>::shrink() {
  if(rozmiar < pojemnosc / 2 && pojemnosc > 0) {
    std::size_t new_capacity = pojemnosc / 2;
    T* nowe_dane = new T[new_capacity];
    
    for(std::size_t i = 0; i < rozmiar; ++i) {
      nowe_dane[i] = dane[i];
    }
    
    delete[] dane;
    dane = nowe_dane;
    pojemnosc = new_capacity;
  }
}

template<typename T>
void DynamicArray<T>::resize(std::size_t new_capacity) {
  // std::cout << "ZMIENIAM ROZMIAR: " << new_capacity << std::endl;
  if (new_capacity == 0) {
    delete[] dane;
    dane = nullptr;
    rozmiar = 0;
    pojemnosc = 0;
    return;
  }

  T* new_data = new T[new_capacity];

  std::size_t ile_do_kopiowania = std::min(rozmiar, new_capacity);
  for (std::size_t i = 0; i < ile_do_kopiowania; ++i) {
    new_data[i] = dane[i];
  }

  delete[] dane;
  dane = new_data;

  pojemnosc = new_capacity;
  rozmiar = ile_do_kopiowania;
}


template<typename T>
void DynamicArray<T>::push_back(T wartosc) {
  if(rozmiar == pojemnosc) {
    resize(pojemnosc == 0 ? 1 : pojemnosc * 2);
  }
  dane[rozmiar++] = wartosc;
}

template<typename T>
void DynamicArray<T>::push_front(T wartosc) {
  if(rozmiar == pojemnosc) {
    resize(pojemnosc == 0 ? 1 : pojemnosc * 2);
  }
  for(std::size_t i = rozmiar; i > 0; --i) {
    dane[i] = dane[i - 1];
  }
  dane[0] = wartosc;
  ++rozmiar;
}

template<typename T>
void DynamicArray<T>::push_at(unsigned int n, T wartosc) {
  if(n > rozmiar) throw std::out_of_range("n jest poza zakresem tablicy");
  if(rozmiar == pojemnosc) {
    resize(pojemnosc == 0 ? 1 : pojemnosc * 2);
  }
  for(std::size_t i = rozmiar; i > n; --i) {
    dane[i] = dane[i - 1];
  }
  dane[n] = wartosc;
  ++rozmiar;
}

template<typename T>
T DynamicArray<T>::remove_back() {
  if(rozmiar == 0) throw std::out_of_range("Tablica jest pusta");
  T wartosc = dane[--rozmiar];
  shrink();
  return wartosc;
}

template<typename T>
T DynamicArray<T>::remove_front() {
  if(rozmiar == 0) throw std::out_of_range("Tablica jest pusta");
  T wartosc = dane[0];
  for(std::size_t i = 0; i < rozmiar - 1; ++i) {
    dane[i] = dane[i + 1];
  }
  --rozmiar;
  shrink();
  return wartosc;
}

template<typename T>
T DynamicArray<T>::remove_at(unsigned int n) {
  if(n >= rozmiar) throw std::out_of_range("n jest poza zakresem tablicy");
  T wartosc = dane[n];
  for(std::size_t i = n; i < rozmiar - 1; ++i) {
    dane[i] = dane[i + 1];
  }
  --rozmiar;
  shrink();
  return wartosc;
}

template<typename T>
int DynamicArray<T>::find(T wartosc) {
  for(std::size_t i = 0; i < rozmiar; ++i) {
    if(dane[i] == wartosc) return i;
  }
  return -1;
}

template<typename T>
T DynamicArray<T>::at_position(unsigned int n) {
  if(n >= rozmiar) throw std::out_of_range("n jest poza zakresem tablicy");
  return dane[n];
}

template<typename T>
void DynamicArray<T>::_show() const {
  std::cout << "[";
  for(std::size_t i = 0; i < rozmiar; ++i) {
    std::cout << dane[i];
    if(i != rozmiar - 1) std::cout << "; ";
  }
  std::cout << "]\n";
}

template<typename T>
T& DynamicArray<T>::operator[](size_t i) {
  // std::cout << "R: " << rozmiar << " P: " << pojemnosc << " i: " << i << "\n";
  if(i >= rozmiar) throw std::out_of_range("Index out of bounds!");
  return dane[i];
}

template<typename T>
DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray& inny) {
  if (this != &inny) {
    T* new_data = nullptr;
    if (inny.pojemnosc > 0) {
      new_data = new T[inny.pojemnosc];
      
      for (std::size_t i = 0; i < inny.rozmiar; ++i) {
        new_data[i] = inny.dane[i];
      }
    }
    
    delete[] dane;
    dane = new_data;
    rozmiar = inny.rozmiar;
    pojemnosc = inny.pojemnosc;
  }
  return *this;
}

#endif
