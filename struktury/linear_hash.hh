#ifndef LINEARHASH_HH
#define LINEARHASH_HH
#include "IHash.hh"

//jest klasyczny problem -- jesli dodam np. „Magdalena”, potem dodam cos o takim samym haszu powiedzmy „Adrian”
//nastepnie usune Magdalene (Adrian jest na kolejnej pozycji, załóżmy) to przy probie wyszukania Adriana szukac go bedzie
//na miejscu (usunietej) Magdaleny, wiec powie ze nic nie ma : ((( (problem nagrobkuw lol)
//dlatego ta modyfikacja pary, klasa ktora ma pare i jeszcze dwie wartosci z interfejsem ukradzionym z pary po czesci hhihihi
class Slot {
private:
  Pair para;
  bool zaj;
  bool usu;

public:
  Slot() : para(Pair(0, L"")), zaj(0), usu(0) {}
  Slot(const Pair& p, bool z, bool u) : para(p), zaj(z), usu(u) {}
  Slot(Slot& s) { para = s.para; zaj = s.zaj; usu = s.usu; }
  
  bool zajete() const { return zaj; }
  bool usuniete() const { return usu; }

  void oznacz_zajete() { zaj = true; }
  void odznacz_zajete() { zaj = false; }
  void oznacz_usuniete() { usu = true; }
  void odznacz_usuniete() { usu = false; }
  
  Slot& operator=(const Slot& other) {
    if (this != &other) {
      para = other.para;
      zaj = other.zaj;
      usu = other.usu;
    }
    return *this;
  }
  void ustaw(int key, const wchar_t* val) {
    para.set_key(key);
    para.set_val(val);
    zaj = true;
    usu = false;
  }

  int get_key() const { return para.get_key(); }
  const wchar_t* get_val() const { return para.get_val(); }
  
  //oddelegowanie do Pair, bo w sumie tak wystarczy
  bool operator==(const Slot& other) const { return para == other.para; }
  bool operator!=(const Slot& other) const { return para != other.para; }
  bool operator<(const Slot& other) const  { return para < other.para; }
  bool operator>(const Slot& other) const  { return para > other.para; }
  bool operator<=(const Slot& other) const { return para <= other.para; }
  bool operator>=(const Slot& other) const { return para >= other.para; }
  friend std::ostream& operator<<(std::ostream& os, const Slot& p);
};
std::ostream& operator<<(std::ostream& os, const Slot& p) {
  //(zaj|usu)(key|val)
  os << "(" << p.zaj << "|" << p.usu << ")" << p.para;
  return os;
}

class LinearStrategy : public HashMapStrategy {
private:
  unsigned int (*hash_fun)(const wchar_t[VAL_SIZE], unsigned int n);
  DynamicArray<Slot> dane;
  int zajete = 0;

  void rehash();
  
public:
  LinearStrategy(unsigned int (*h)(const wchar_t[VAL_SIZE], unsigned int n), int wielkosc = 2) {
    hash_fun = h;
    for(int i = 0; i < wielkosc; i++) {
      dane.push_back(Slot());
    }
  };
  ~LinearStrategy() {}
  
  //wstawianie -- na polu zwroconym przez haszującą, albo na kolejnym (+1) jesli wolne, jesli nie to jeszcze dalej
  //i idzie az do konca tablicy, potem idzie od poczatku znowu do tamtej pozycji
  //de facto przy zachowaniu maks. 70% pojemnosci to zawsze gwarantuje miejsce, ALE i tak moze zwrocic false
  bool insert(const int val, const wchar_t* key) override;
  bool insert(Pair p) override {
    return insert(p.get_key(), p.get_val());
  }
  bool insert(Slot& s) {
    return insert(s.get_key(), s.get_val());
  }
  //usuniecie -- haszujemy klucz i albo jest on na tym indeksie (wspaniale) albo na nastepnym (dobrze) albo jeszcze dalej (srednio)
  //albo jeszcze... albo w ogole go nie ma (okropnie)
  bool remove(const wchar_t* klucz) override;
  int get_val(const wchar_t* klucz) override; //zwraca wartosc jaka powiazana (lub wyjatek)
  size_t search(const wchar_t* klucz) override; //zwraca indeks gdzie przechowywane (lub wyjatek)

  void _show() const override;
  size_t size() const override;
};

void LinearStrategy::rehash() {
  std::cout << "Rehash!" << std::endl;
  DynamicArray<Slot> stare = dane;
  size_t rozmiar = dane.get_size();
  zajete = 0; //zeby wstawialo normalnie, to sie i tak ustawi na dobre pozniej
  for(size_t i = 0; i < rozmiar*2; i++) { //zeruje calosc
    if(i < rozmiar) {
      dane[i] = Slot();
    }
    else
      dane.push_back(Slot());
  }
  
  for(size_t i=0;i<rozmiar;i++) { //dla kazdego ze starych
    if(stare[i].zajete()) {
      size_t indeks = hash_fun(stare[i].get_val(), rozmiar*2);
      size_t startowy = indeks;
      //szukamy wolnego pola, ale wewnatrz tablicy
      while (dane[indeks].zajete() && !dane[indeks].usuniete()) {
	indeks = (indeks + 1) % (rozmiar*2);
	if(indeks == startowy)
	   throw std::out_of_range("Coś poszło nie tak przy rehashowaniu, nie odnaleziono miejsca!");
      }

      //wiec trafiło gdzieś w tablicę
      dane[indeks].ustaw(stare[i].get_key(), stare[i].get_val());
      zajete++;
    }
  }
}

bool LinearStrategy::insert(const int val, const wchar_t* key) {
  // std::cout << "insert, dane.get_size() = " << dane.get_size() << std::endl;
  // std::cout << "                 zajete = " << zajete << std::endl;
  unsigned int klucz = hash_fun(key, dane.get_size());
  unsigned int startowy = klucz; //bo bedziemy przegladac do konca, potem od poczatku do tego
  size_t rozmiar = dane.get_size();
  bool zwrot = true;
  //szukamy wolnego pola, ale wewnatrz tablicy
  while (dane[klucz].zajete() && !dane[klucz].usuniete()) {
    klucz = (klucz + 1) % rozmiar;
    if(klucz == startowy)
      return false;
    if(std::wcsncmp(key, dane[klucz].get_val(), VAL_SIZE) == 0) {
      zwrot = false;
      //Pair para(val, key);
      //std::cout << "NADPISANIE WARTOSCI " << para << std::endl;
      break;
    }
  }
  dane[klucz].ustaw(val, key);

  zajete++;
  if((zajete / (float)dane.get_size()) >= 0.7)
    rehash();
  return zwrot; 
}

bool LinearStrategy::remove(const wchar_t* klucz) {
  //prezentacja systemu trajkacz
  size_t indeks;
  try {         
    indeks = search(klucz);
  } 
  catch (std::out_of_range&) { //maly troll
    throw std::out_of_range("Nie ma takiego klucza w zbiorze!");
  }

  zajete--;
  dane[indeks].oznacz_usuniete();
  dane[indeks].odznacz_zajete();
  return true;
}

int LinearStrategy::get_val(const wchar_t* klucz){
  size_t indeks;
  try {         
    indeks = search(klucz);
  } 
  catch (std::out_of_range&) {
    throw std::out_of_range("Nie ma takiego klucza w zbiorze!");
  }
    
  return dane[indeks].get_key();
}

size_t LinearStrategy::search(const wchar_t* klucz){  
  size_t indeks = hash_fun(klucz, dane.get_size());
  size_t startowy = indeks;
  while (true) {
    const Slot& slot = dane[indeks];

    if (slot.zajete() && !slot.usuniete()) {
      if (std::wcsncmp(klucz, slot.get_val(), VAL_SIZE) == 0) {
	return indeks;
      }
    }

    indeks = (indeks + 1) % dane.get_size();
    if (indeks == startowy)
      break;
  }

  throw std::out_of_range("Nie ma takiego klucza w zbiorze!");
}  

void LinearStrategy::_show() const { dane._show(); }

size_t LinearStrategy::size() const { return dane.get_size(); }

#endif
