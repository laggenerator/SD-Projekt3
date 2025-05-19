#ifndef LINKHASH_HH
#define LINKHASH_HH
#include "IHash.hh"
#include "pair.hh"
#include "dynamic_array.hh"
#include "list.hh"

//łańcuchowanie sigmowanie
//dodaje na koniec listy 
class LinkStrategy : public HashMapStrategy {
  unsigned int (*hash_fun)(const wchar_t[VAL_SIZE], unsigned int n);
  DynamicArray<List<Pair>> dane;
  unsigned int zajete = 0;
  void rehash(){
    size_t rozmiar = dane.get_size();
    std::cout << "resize: " << rozmiar << std::endl;
    dane.resize(rozmiar*2);
    for(size_t i=rozmiar;i<rozmiar*2;i++){
      dane.push_back(List<Pair>());
    }
    Pair p;
    unsigned int klucz, rozmiarkubla;
    for(size_t i=0;i<rozmiar;i++){
      auto& lista = dane[i];
      rozmiarkubla = lista.get_size();
      for(unsigned int j=0;j<rozmiarkubla;j++){
        // std::cout << "ROZMIAR LISTY: " << lista.get_size() << std::endl;
        // dane._show();
        p = lista.remove_front();
        // std::cout << p << " " << hash_fun(p.get_val(), rozmiar) << " " << hash_fun(p.get_val(), rozmiar*2) << std::endl;
        klucz = hash_fun(p.get_val(), rozmiar*2);
        dane[klucz].push_back(p);
      }
    }
  }
  public:
  //pojawia sie potrzeba zainicjalizowania poczatkowo tablicy dynamicznej, np do 1000
  LinkStrategy(unsigned int (*h)(const wchar_t[VAL_SIZE], unsigned int n), int wielkosc = 2) {
    hash_fun = h;
    for(int i = 0; i < wielkosc; i++) {
      dane.push_back(List<Pair>());
    }
  };
  ~LinkStrategy() {}
  bool insert(const int val, const wchar_t* key) {
    unsigned int klucz = hash_fun(key, dane.get_size());
    size_t rozmiar = dane.get_size();
    Pair p(val, key);
    if(dane[klucz].get_size() == 0) zajete++;
    dane[klucz].push_back(p);
    // std::cout << "zajete: " << zajete << " na: " << dane.get_size() << std::endl;
    // dane._show();
    // sprawdzenie rozmiaru
    if((zajete / (float)rozmiar) >= 0.7){
      rehash();
    }

    // std::cout << "Wstawiam ziutka: " << klucz << std::endl;
    return true; //nie moze sie nie udac :)
  }
  bool insert(Pair p) {
    return insert(p.get_key(), p.get_val());
  }
  bool remove(const wchar_t* klucz) { //trzeba zmniejszac zajetosc
    auto &lista = dane[hash_fun(klucz, dane.get_size())]; // Jakbyśmy zmienili co trzymamy w środku to sie dostosuje :) 
    size_t idx = search(klucz);
    if(idx < lista.get_size()){
      lista.remove_at(idx);
      if(lista.get_size() == 0){
        zajete--;
      }
      return true;
    }
    return false;
  }

  int get_val(const wchar_t* klucz){
    unsigned int idk = hash_fun(klucz, dane.get_size());
    auto &lista = dane[idk];
    if(lista.get_size() == 0) throw std::out_of_range("Nie ma takiego klucza w zbiorze!");
    size_t idx = search(klucz);
    if((idx < lista.get_size())){

      return lista.at_position(idx)->value().get_key();
    }
    throw std::out_of_range("Nie ma takiego klucza w zbiorze!");
  }
  
  virtual size_t search(const wchar_t* klucz){
    unsigned int idk = hash_fun(klucz, dane.get_size());
    if(idk >= dane.get_size()) throw std::out_of_range("Nie ma takiego klucza w zbiorze!");
    auto &lista = dane[idk];
    if(lista.get_size() == 0) throw std::out_of_range("Przeszukiwany kubełek jest pusty!");
    Pair dummy(0, klucz);
    return lista.find_index(dummy);
  }  

  void _show() const { dane._show(); };
  size_t size() const { return dane.get_size(); };
};

#endif
