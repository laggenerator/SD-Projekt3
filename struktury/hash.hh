#ifndef HASH_HH
#define HASH_HH
#include "pair.hh"
#include "dynamic_array.hh"
#include "list.hh"

//klasa wirtualna, po ktorej dziedzicza strategie rozne trzy
//nastepnie dawana jako argument do konstruktora HashMap
class HashMapStrategy {
public:
  virtual ~HashMapStrategy() = default;
  virtual bool insert(int val, const wchar_t* key) = 0;
  virtual bool insert(Pair p) = 0;
  virtual bool remove(wchar_t* klucz) = 0;

  virtual void _show() const = 0;
  virtual size_t size() const = 0;
};

//łańcuchowanie sigmowanie
//dodaje na koniec listy 
class LinkStrategy {
  unsigned int (*hash_fun)(const wchar_t[VAL_SIZE]);
  DynamicArray<List<int>> dane;
  public:
  //pojawia sie potrzeba zainicjalizowania poczatkowo tablicy dynamicznej, np do 1000
  LinkStrategy(unsigned int (*h)(const wchar_t[VAL_SIZE])) {
    hash_fun = h;
    for(int i = 0; i < 1e6; i++) {
      List<int> l0;
      dane.push_back(l0);
    }
  };
  ~LinkStrategy() {}
  bool insert(const int val, const wchar_t* key) {
    dane[hash_fun(key)].push_back(val);
    return true; //nie moze sie nie udac :)
  }
  bool insert(Pair p) {
    return insert(p.get_key(), p.get_val());
  }
  bool remove(wchar_t* klucz) {
    return true; //nie usuwam :)))))))
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

#endif // HASH_HH
