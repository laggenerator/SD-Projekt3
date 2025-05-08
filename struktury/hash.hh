#ifndef HASH_HH
#define HASH_HH
#include "pair.hh"
#include "dynamic_array.hh"
#include "list.hh"
#include <stdexcept>


//klasa wirtualna, po ktorej dziedzicza strategie rozne trzy
//nastepnie dawana jako argument do konstruktora HashMap
class HashMapStrategy {
public:
  virtual ~HashMapStrategy() = default;
  virtual bool insert(int val, const wchar_t* key) = 0;
  virtual bool insert(Pair p) = 0;
  virtual bool remove(wchar_t* klucz) = 0;
  virtual unsigned int get_val(wchar_t* klucz) = 0;
  virtual size_t search(wchar_t* klucz) = 0;


  virtual void _show() const = 0;
  virtual size_t size() const = 0;
};

//łańcuchowanie sigmowanie
//dodaje na koniec listy 
class LinkStrategy : public HashMapStrategy {
  unsigned int (*hash_fun)(const wchar_t[VAL_SIZE]);
  DynamicArray<List<Pair>> dane;
  public:
  //pojawia sie potrzeba zainicjalizowania poczatkowo tablicy dynamicznej, np do 1000
  LinkStrategy(unsigned int (*h)(const wchar_t[VAL_SIZE])) {
    hash_fun = h;
    for(int i = 0; i < 2; i++) {
      List<Pair> l0;
      dane.push_back(l0);
    }
  };
  ~LinkStrategy() {}
  bool insert(const int val, const wchar_t* key) {
    unsigned int klucz = hash_fun(key);
    size_t rozmiar = dane.size();
    if(klucz > dane.size() -1){
      dane.resize(klucz+1);

      for(size_t i=rozmiar;i<=klucz;i++){
        List<Pair> l0;
        dane.push_back(l0);
      }
    }

    Pair p(val, key);
    dane[klucz].push_back(p);
    // std::cout << "Wstawiam ziutka: " << klucz << std::endl;
    return true; //nie moze sie nie udac :)
  }
  bool insert(Pair p) {
    return insert(p.get_key(), p.get_val());
  }
  bool remove(wchar_t* klucz) {
    auto &lista = dane[hash_fun(klucz)]; // Jakbyśmy zmienili co trzymamy w środku to sie dostosuje :) 
    size_t idx = search(klucz);
    if(idx < lista.get_size()){
      lista.remove_at(idx);
      return true;
    }
    return false;
  }

  unsigned int get_val(wchar_t* klucz){
    unsigned int idk = hash_fun(klucz);
    if(idk >= dane.size()) throw std::out_of_range("Nie ma takiego klucza w zbiorze!");
    auto &lista = dane[idk];
    if(lista.get_size() == 0) throw std::out_of_range("Nie ma takiego klucza w zbiorze!");
    size_t idx = search(klucz);
    if((idx < lista.get_size())){
      return lista.at_position(idx)->value().get_key();
    }
  }
  
  virtual size_t search(wchar_t* klucz){
    unsigned int idk = hash_fun(klucz);
    if(idk >= dane.size()) throw std::out_of_range("Nie ma takiego klucza w zbiorze!");
    auto &lista = dane[idk];
    if(lista.get_size() == 0) throw std::out_of_range("Przeszukiwany kubełek jest pusty!");
    Pair dummy(0, klucz);
    return lista.find_index(dummy);
  }  

  void _show() const { dane._show(); };
  size_t size() const { return dane.size(); };
};

//liniowe
class LinearStrategy {

};

//kukułka
class CuckooStrategy {
  
};

unsigned int hash1(const wchar_t tab[VAL_SIZE]) {
  return 1;
}

unsigned int modulo_hash(const wchar_t tab[VAL_SIZE]){
  const unsigned int p = 31; // Internet jej lubi używać nie wiem czemu
  const unsigned int duze_modulo = 1e6;
  unsigned int hash = 0;
  for(int i=0;i<VAL_SIZE;i++){
    hash = (hash + (tab[i] % p)) % duze_modulo;
  }
  return hash;
}

#endif // HASH_HH
