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
  virtual int generate_key(const wchar_t* klucz) = 0;
  
  
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
unsigned int modulo_hash_seed(const wchar_t* tab, unsigned int n, unsigned int seed ){
  const unsigned int p = (n > 50 ? n/13 : n); // Internet jej lubi używać nie wiem czemu
  unsigned int hash = seed;
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

unsigned int fnv_1_seed(const wchar_t* str, unsigned int modulo, unsigned int seed = 0x811c9dc5) {
  const unsigned int fnv_prime = 0x01000193;
  unsigned int hash = seed;

  while (*str) {
    hash ^= static_cast<unsigned int>(*str++);
    hash *= fnv_prime;
  }

  return hash % modulo;
}
unsigned int fnv_1(const wchar_t* str, unsigned int modulo) {
  return fnv_1_seed(str, modulo, 0x811c9dc5);
}

unsigned int djb2_seed(const wchar_t* str, unsigned int modulo, unsigned int seed = 5381) {
  unsigned int hash = seed;
  while (*str) {
    hash = ((hash << 5) + hash) + static_cast<unsigned int>(*str++); // hash * 33 + c
  }
  return hash % modulo;
}
unsigned int djb2(const wchar_t* str, unsigned int modulo) {
  return djb2_seed(str, modulo, 5381);
}
// https://www.youtube.com/watch?v=cwJKjuyLv80
unsigned int murmur3_seed(const wchar_t* klucz, unsigned int modulo, unsigned int seed = 0) {
  const unsigned int* data = reinterpret_cast<const unsigned int*>(klucz);
  unsigned int h = seed;
  const unsigned int c1 = 0xcc9e2d51;
  const unsigned int c2 = 0x1b873593;
  unsigned int len = 0;
  
  while (klucz[len] != L'\0') len++;
  
  for (unsigned int i = 0; i < len; i++) {
    unsigned int k = data[i];
    
    k *= c1;
    k = (k << 15) | (k >> 17);  
    k *= c2;
    
    h ^= k;
    h = (h << 13) | (h >> 19);
    h = h * 5 + 0xe6546b64;
  }
  
  
  h ^= len * 4;
  h ^= h >> 16;
  h *= 0x85ebca6b;
  h ^= h >> 13;
  h *= 0xc2b2ae35;
  h ^= h >> 16;
  
  return h % modulo;
}
unsigned int murmur3(const wchar_t* klucz, unsigned int modulo){
  return murmur3_seed(klucz, modulo);
}
#endif
