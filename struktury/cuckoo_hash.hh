#ifndef CUCKOO_HASH
#define CUCKOO_HASH
#include "IHash.hh"
#include "linear_hash.hh" //slot
#include "dynamic_array.hh"

//Regula jakiej chce sie tutaj trzymac to to, ze dla uzytkownika to jest JEDNA tablica, dwie sa tylko w implementacji
//wiec podawany rozmiar jest dzielony na pół (musi byc wiec parzysty) i po prostu wieksze od polowy indeksy sa w drugiej
//tabeli, reszta w pierwszej
class CuckooStrategy : public HashMapStrategy {
private:
  DynamicArray<Slot> tab1;
  DynamicArray<Slot> tab2;
  unsigned int (*hash_fun1)(const wchar_t[VAL_SIZE], unsigned int);
  unsigned int (*hash_fun2)(const wchar_t[VAL_SIZE], unsigned int); 
  size_t zajete;
public:
  CuckooStrategy(unsigned int (*h1)(const wchar_t[VAL_SIZE], unsigned int n), unsigned int (*h2)(const wchar_t[VAL_SIZE], unsigned int n), size_t wielkosc = 2);
  ~CuckooStrategy() {};
  bool insert(int val, const wchar_t* key) override;
  bool insert(Pair p) override;
  bool insert(Slot s);
  bool remove(const wchar_t* klucz) override;
  int get_val(const wchar_t* klucz) override;
  size_t search(const wchar_t* klucz) override;
  
  
  void _show() const override;
  size_t size() const override;
};

//konstruktor przypisuje funkcje haszujace, alokuje podana ilosc miejsca i uuzupelnia zerowo lol (obie tablice maja rowny rozmiar)
//jesli podadza nieparzysty rozmiar to zwiekszamy o jeden
CuckooStrategy::CuckooStrategy(unsigned int (*h1)(const wchar_t[VAL_SIZE], unsigned int n), unsigned int (*h2)(const wchar_t[VAL_SIZE], unsigned int n), size_t wielkosc) : hash_fun1(h1), hash_fun2(h2) {
  if(wielkosc%2) wielkosc++;
  zajete = 0;
  tab1.resize(wielkosc/2);
  tab2.resize(wielkosc/2);
  for(size_t i = 0; i < wielkosc/2; ++i) {
    tab1.push_back(Slot());
    tab2.push_back(Slot());
  }
}

//próbujemy wstawic na wyznaczona pozycje do tab1, jesli jest zajete to
//zapamietujemy co tam jest, wstawiamy to ktore chcemy do tab1, a temu zapamietanemu
//szukamy miejsca w tab2, jesli jest zajete ..., a temu zapamietanemu szukamy miejsca w tab1
bool CuckooStrategy::insert(int val, const wchar_t* key) {
  Slot wstawiany(Pair(val, key), 1, 0);
  DynamicArray<Slot>* tablica = &tab1;
  unsigned int (*haszor)(const wchar_t[VAL_SIZE], unsigned int) = hash_fun1;
  size_t indeks = haszor(key, size()/2);
  int licznikCyklu = size()*2;
  //jesli jest wolne lub jesli jest usuniete mozna wstawic, patrzymy kiedy NIE mozna
  while(!(!(*tablica)[indeks].zajete() || (*tablica)[indeks].usuniete())) {
    Slot stary = (*tablica)[indeks];
    (*tablica)[indeks] = wstawiany;
    wstawiany = stary;
    tablica = (tablica == &tab1 ? &tab2 : &tab1);
    haszor = (haszor == hash_fun1 ? hash_fun2 : hash_fun1);
    indeks = haszor(wstawiany.get_val(), size()/2);
    if(--licznikCyklu <= 0) {
      std::cout << "KONIEC CYKLU" << std::endl;
      return false; //CYKL! przerypane jest
    }
  }
  (*tablica)[indeks] = wstawiany;
  zajete++;
  //if(...)
    //rehash
  return true;
}

bool CuckooStrategy::insert(Pair p) {
  return insert(p.get_key(), p.get_val());
}

bool CuckooStrategy::insert(Slot s) {
  return insert(s.get_key(), s.get_val());
}

bool CuckooStrategy::remove(const wchar_t* klucz) {
  size_t indeks;
  try {         
    indeks = search(klucz);
  } 
  catch (std::out_of_range&) { //maly troll
    throw std::out_of_range("Nie ma takiego klucza w zbiorze!");
  }

  zajete--;
  DynamicArray<Slot>* tablica = (indeks >= size() ? &tab2 : &tab1);
  (*tablica)[indeks%(size()/2)].oznacz_usuniete();
  (*tablica)[indeks%(size()/2)].odznacz_zajete();

  return true;
}

int CuckooStrategy::get_val(const wchar_t* klucz) {
  size_t indeks;
  try {         
    indeks = search(klucz);
  } 
  catch (std::out_of_range&) { //maly troll
    throw std::out_of_range("Nie ma takiego klucza w zbiorze!");
  }

  DynamicArray<Slot>* tablica = (indeks >= size() ? &tab2 : &tab1);
  return (*tablica)[indeks%(size()/2)].get_key();
}

//zgodnie z przyjetym zalozeniem, jesli rozmiar to np 200, to indeksy 0-99 sa w tab1, a 100-199 w tab2
size_t CuckooStrategy::search(const wchar_t* klucz) {
  size_t index = hash_fun1(klucz, size()/2); //najpierw szukamy w 1
  if(tab1[index].zajete() && (wcscmp(tab1[index].get_val(), klucz) == 0))
    return index;

  index = hash_fun2(klucz, size()/2); //potem szukamy w 2
  if(tab2[index].zajete() && (wcscmp(tab2[index].get_val(), klucz) == 0))
    return size() + index;

  throw std::out_of_range("Nie ma takiego klucza w zbiorze!");
}

void CuckooStrategy::_show() const { tab1._show(); tab2._show(); }

size_t CuckooStrategy::size() const { return 2*tab1.get_size(); /*oba maja miec taki sam*/ };

#endif
