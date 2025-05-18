#ifndef HASH_INTERFACE_HH
#define HASH_INTERFACE_HH
#include "pair.hh"

//klasa wirtualna, po ktorej dziedzicza strategie rozne trzy
//nastepnie dawana jako argument do konstruktora HashMap
class HashMapStrategy {
  public:
  virtual ~HashMapStrategy() = default;
  virtual bool insert(int val, const wchar_t* key) = 0;
  virtual bool insert(Pair p) = 0;
  virtual bool remove(const wchar_t* klucz) = 0;
  virtual int get_val(const wchar_t* klucz) = 0;
  virtual size_t search(const wchar_t* klucz) = 0;
  
  
  virtual void _show() const = 0;
  virtual size_t size() const = 0;
};

unsigned int hash1(const wchar_t tab[VAL_SIZE], unsigned int n) {
  return 1;
}

unsigned int modulo_hash(const wchar_t* tab, unsigned int n){
  const unsigned int p = (n > 50 ? n/13 : n); // Internet jej lubi używać nie wiem czemu
  unsigned int hash = 0;
  for(int i=0;i<VAL_SIZE;i++){
    if(tab[i] == '\0') break;
    hash = (hash + (tab[i] % p));
  }
  return hash % n;
}

unsigned int jakis_hash(const wchar_t tab[VAL_SIZE], unsigned int n){
  int dlugosc=0;
  unsigned int hash = 0;
  // Dlugosc wartosci
  while(tab[dlugosc] != '\0'){
    hash = (hash * 227) ^ static_cast<unsigned int> (tab[dlugosc]);
    dlugosc++;
  }
  return hash%n;
}

unsigned int fnv_1(const wchar_t* str, unsigned int modulo) {
  const unsigned int fnv_prime = 0x01000193;
  unsigned int hash = 0x811C9DC5;

  while (*str) {
    hash ^= static_cast<unsigned int>(*str++);
    hash *= fnv_prime;
  }

  return hash % modulo;
}

unsigned int djb2(const wchar_t* str, unsigned int modulo) {
  unsigned int hash = 5381;
  while (*str) {
    hash = ((hash << 5) + hash) + static_cast<unsigned int>(*str++); // hash * 33 + c
  }
  return hash % modulo;
}

#endif
