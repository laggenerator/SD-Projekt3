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
  virtual bool remove(const wchar_t* klucz) = 0;
  virtual unsigned int get_val(const wchar_t* klucz) = 0;
  virtual size_t search(const wchar_t* klucz) = 0;


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
    for(int i = 0; i < 1e6; i++) {
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
  bool remove(const wchar_t* klucz) {
    auto &lista = dane[hash_fun(klucz)]; // Jakbyśmy zmienili co trzymamy w środku to sie dostosuje :) 
    size_t idx = search(klucz);
    if(idx < lista.get_size()){
      lista.remove_at(idx);
      return true;
    }
    return false;
  }

  unsigned int get_val(const wchar_t* klucz){
    unsigned int idk = hash_fun(klucz);
    if(idk >= dane.size()) throw std::out_of_range("Nie ma takiego klucza w zbiorze!");
    auto &lista = dane[idk];
    if(lista.get_size() == 0) throw std::out_of_range("Nie ma takiego klucza w zbiorze!");
    size_t idx = search(klucz);
    if((idx < lista.get_size())){
      return lista.at_position(idx)->value().get_key();
    }
  }
  
  virtual size_t search(const wchar_t* klucz){
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
//ogolnie to na poczatku ma rozmiar dwa, potem przy wstawianiu haszuje i potem liniowo próbuje wstawić na kolejne (+1)
//jesli wolne to fajnie, jesli nie to niefajnie
class LinearStrategy {
private:
  unsigned int (*hash_fun)(const wchar_t[VAL_SIZE]);
  DynamicArray<Pair> dane;
public:
  LinearStrategy(unsigned int (*h)(const wchar_t[VAL_SIZE])) {
    hash_fun = h;
    for(int i = 0; i < 2; i++) {
      Pair p0;
      dane.push_back(p0);
    }
  };
  ~LinearStrategy() {}
  
  //wstawianie -- na polu zwroconym przez haszującą, albo na kolejnym (+1) jesli wolne, jesli nie to jeszcze dalej
  //ewentualnie az do konca tablicy haszującej, gdzie ją już rozszerzy specjalnie
  bool insert(const int val, const wchar_t* key);
  bool insert(Pair p) {
    return insert(p.get_key(), p.get_val());
  }
  //usuniecie -- haszujemy klucz i albo jest on na tym indeksie (wspaniale) albo na nastepnym (dobrze) albo jeszcze dalej (srednio)
  //albo jeszcze... albo w ogole go nie ma (okropnie)
  bool remove(wchar_t* klucz);
  int get_val(wchar_t* klucz); //zwraca wartosc jaka powiazana (lub wyjatek)
  size_t search(wchar_t* klucz); //zwraca indeks gdzie przechowywane (lub wyjatek)

  void _show() const { dane._show(); };
  size_t size() const { return dane.size(); };
};

bool LinearStrategy::insert(const int val, const wchar_t* key) {
  //std::cout << "Dodaję val = " << val << " key = " << key << std::endl;
  if(key[0] == '\0') //proba wstawienia pustej wartosci, a w naszej logice to wolne pole wiec NIE MOŻE
    throw std::out_of_range("Próba wstawienia nielegalnej wartości! Pusty łańuch znaków.");

  unsigned int klucz = hash_fun(key);
  //std::cout << "indeks: " << klucz << std::endl;
  size_t rozmiar = dane.size();

  //szukamy wolnego pola, ale wewnatrz tablicy
  while(klucz < rozmiar && dane[klucz].get_val()[0] != '\0') {
    klucz++;
  }

  //nie trafił wewnatrz tablicy :c
  if(klucz > rozmiar-1) {
    //std::cout << "rozszerzanie" << std::endl;
    dane.resize(klucz+1);
    for(size_t i = rozmiar; i <= klucz; ++i){
      Pair p0;
      dane.push_back(p0);
    }
  }

  //wiec trafiło gdzieś w tablicę
  Pair p(val, key);
  dane[klucz] = p;
  //std::cout << "dane[" << klucz << "] = " << p << std::endl;
  return true; 
}

bool LinearStrategy::remove(wchar_t* klucz) {
  //prezentacja systemu trajkacz
  size_t indeks;
  try {         
    indeks = search(klucz);
  } 
  catch (std::out_of_range&) { //maly troll
    throw std::out_of_range("Nie ma takiego klucza w zbiorze!");
  }
    
  dane[indeks] = Pair(); //i ustawia na puste ("/0", 0)
  return true;
}

int LinearStrategy::get_val(wchar_t* klucz){
  size_t indeks;
  try {         
    indeks = search(klucz);
  } 
  catch (std::out_of_range&) {
    throw std::out_of_range("Nie ma takiego klucza w zbiorze!");
  }
    
  return dane[indeks].get_key();
}

size_t LinearStrategy::search(wchar_t* klucz){
  if (klucz[0] == '\0')
    throw std::out_of_range("Pusty klucz — nielegalny.");
    
  size_t indeks = hash_fun(klucz);
  while(std::wcsncmp(klucz, dane[indeks].get_val(), VAL_SIZE) != 0) {
    indeks++;
    if(indeks == dane.size()) //nie znalazlo
      throw std::out_of_range("Nie ma takiego klucza w zbiorze!");
  }
    
  //jednak znalazlo
  return indeks;
}  

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
