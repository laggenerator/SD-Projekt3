#ifndef CUCKOO_HASH
#define CUCKOO_HASH
#include <limits.h>
#include <random>
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
  unsigned int (*hash_fun1)(const wchar_t[VAL_SIZE], unsigned int, unsigned int);
  unsigned int (*hash_fun2)(const wchar_t[VAL_SIZE], unsigned int, unsigned int); 
  size_t zajete;
  size_t seed[2]; //ziarna dla funkcji haszujacych, ich modyfikacja powinna zmieniac funkcje haszujaca

  void rehash(bool zwiekszaj); //ile razy zmienic rozmiar tablicy
  bool try_inserting(DynamicArray<Slot>* a1, DynamicArray<Slot>* a2, Slot value);
public:
  CuckooStrategy(unsigned int (*h1)(const wchar_t[VAL_SIZE], unsigned int n, unsigned int seed), unsigned int (*h2)(const wchar_t[VAL_SIZE], unsigned int n, unsigned int seed), size_t wielkosc = 2);
  ~CuckooStrategy() {};
  bool insert(int val, const wchar_t* key) override;
  bool insert(Pair p) override;
  bool insert(Slot s);
  bool remove(const wchar_t* klucz) override;
  int get_val(const wchar_t* klucz) override;
  size_t search(const wchar_t* klucz) override;
  int generate_key(const wchar_t* klucz)  override; //ale ktory zwrocic???, zwraca pierwszy
  
  
  void _show() const override;
  size_t size() const override;
};

void CuckooStrategy::rehash(bool zwiekszaj) {
  std::random_device rd;
  std::default_random_engine generator(rd());
  std::uniform_int_distribution<int> distribution(0,INT_MAX);
  //mam nadzieje, ze operatory = i konstruktory dobrze dzialaja
  size_t rozmiar = (zwiekszaj ? size() : size()/2);
  //tablice nowe, ktore (byc moze) dostapia zaszczytu zostania nowym slownikiem
  DynamicArray<Slot> nowe1; nowe1.resize(rozmiar);
  DynamicArray<Slot> nowe2; nowe2.resize(rozmiar);
  for(size_t i = 0; i < rozmiar; i++) { //wstawia calosc pretendenta
    nowe1.push_back(Slot());
    nowe2.push_back(Slot());
  }

  bool jeszcze_raz;
  do {
    jeszcze_raz = false;
    for(size_t i = 0; i < rozmiar; i++) { //zeruje calosc pretendenta
      nowe1[i] = Slot();
      nowe2[i] = Slot();
    }
    seed[0] = distribution(generator);
    seed[1] = distribution(generator);
    // std::cout << "testuje seedy: " << seed[0] << " " << seed[1] << std::endl;
    // std::cout << rozmiar << std::endl;
    // int licznik = 0;
    for(size_t i=0;i<rozmiar;i++) { //dla kazdego ze starych
      if(tab1[i].zajete()) {
	if(!try_inserting(&nowe1, &nowe2, tab1[i])) {
	  //std::cerr << " :( >... " << std::endl;
	  jeszcze_raz = true;
	  break; //zawiodly nowe seedy
	}
      }
      if(tab2[i].zajete()) {
	if(!try_inserting(&nowe1, &nowe2, tab2[i])) {
	  //std::cerr << " :( >... " << std::endl;
	  jeszcze_raz = true;
	  break; //zawiodly nowe seedy
	}
      }
      // licznik++;
    }
    // std::cout << "wyjscie!" << std::endl;
    // std::cout << "licznik: " << licznik << std::endl;
  } while(jeszcze_raz);

  std::cout << "zapisanie!" << std::endl;
  std::cout << "znalezione seedy: " << seed[0] << " " << seed[1] << std::endl;
  std::cout << "zajetosc: " << zajete << std::endl;
  tab1 = nowe1;
  tab2 = nowe2;
}

//pomocnicza do rehaszowania, probuje do przekazanych wskaznikow na dynamic_arraye wstawic wartosc zgodnie z haszorami
//sama w sobie nie wywoluje rehaszowania, tylko sprawdza czy sie udalo
bool CuckooStrategy::try_inserting(DynamicArray<Slot>* a1, DynamicArray<Slot>* a2, Slot value) {
  Slot wstawiany = value;
  //detekcja duplikatu
  if((*a1)[hash_fun1(wstawiany.get_val(), size()/2, seed[0])] == wstawiany ||
     (*a2)[hash_fun2(wstawiany.get_val(), size()/2, seed[1])] == wstawiany) {
    std::cout << "podobnoż duplikat: " << wstawiany << std::endl;
    throw std::logic_error("Proba wstawienia duplikatu!");
  }
  
  DynamicArray<Slot>* tablica = a1;
  unsigned int (*haszor)(const wchar_t[VAL_SIZE], unsigned int, unsigned int) = hash_fun1;
  unsigned int ziarno = seed[0];
  size_t indeks = haszor(wstawiany.get_val(), size()/2, ziarno);
  int licznikCyklu = size()*2;
  //jesli jest wolne lub jesli jest usuniete mozna wstawic, patrzymy kiedy NIE mozna
  while ((*tablica)[indeks].zajete() && !(*tablica)[indeks].usuniete()) {
    Slot stary = (*tablica)[indeks];
    (*tablica)[indeks] = wstawiany;
    wstawiany = stary;
    tablica = (tablica == a1 ? a2 : a1);
    haszor = (haszor == hash_fun1 ? hash_fun2 : hash_fun1);
    ziarno = (ziarno == seed[0] ? seed[1] : seed[0]); 
    indeks = haszor(wstawiany.get_val(), size()/2, ziarno);
    if(--licznikCyklu <= 0) {
      // std::cout << "KONIEC CYKLU W POMOCNICZEJ" << std::endl;
      // std::cout << "seed: [" << seed[0] << "; " << seed[1] << "] " << std::endl;
      return false; //CYKL! przerypane jest
    }
  }
  (*tablica)[indeks] = wstawiany;
  return true;
}

//konstruktor przypisuje funkcje haszujace, alokuje podana ilosc miejsca i uuzupelnia zerowo lol (obie tablice maja rowny rozmiar)
//jesli podadza nieparzysty rozmiar to zwiekszamy o jeden
CuckooStrategy::CuckooStrategy(unsigned int (*h1)(const wchar_t[VAL_SIZE], unsigned int n, unsigned int s), unsigned int (*h2)(const wchar_t[VAL_SIZE], unsigned int n, unsigned int s), size_t wielkosc) : hash_fun1(h1), hash_fun2(h2) {
  if(wielkosc%2) wielkosc++;
  zajete = 0;
  tab1.resize(wielkosc/2);
  tab2.resize(wielkosc/2);
  seed[0] = 0; seed[1] = 0;
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
  //detekcja duplikatu
  if(tab1[hash_fun1(key, size()/2, seed[0])] == wstawiany ||
     tab2[hash_fun2(key, size()/2, seed[1])] == wstawiany) {
    throw std::logic_error("Proba wstawienia duplikatu!");
  }
							  
  DynamicArray<Slot>* tablica = &tab1;
  unsigned int (*haszor)(const wchar_t[VAL_SIZE], unsigned int, unsigned int) = hash_fun1;
  unsigned int ziarno = seed[0];
  size_t indeks = haszor(key, size()/2, ziarno);
  int licznikCyklu = size()*2;
  //jesli jest wolne lub jesli jest usuniete mozna wstawic, patrzymy kiedy NIE mozna
  while ((*tablica)[indeks].zajete() && !(*tablica)[indeks].usuniete()) {
    Slot stary = (*tablica)[indeks];
    (*tablica)[indeks] = wstawiany;
    wstawiany = stary;
    tablica = (tablica == &tab1 ? &tab2 : &tab1);
    haszor = (haszor == hash_fun1 ? hash_fun2 : hash_fun1);
    ziarno = (ziarno == seed[0] ? seed[1] : seed[0]); 
    indeks = haszor(wstawiany.get_val(), size()/2, ziarno);
    if(--licznikCyklu <= 0) {
      std::cout << "KONIEC CYKLU W ZWYKLEJ" << std::endl;
      std::cout << wstawiany << std::endl;
      rehash(0);
      insert(wstawiany); //zeby nie zapomniec (jeden osiem L)
      return true;
    }
  }
  (*tablica)[indeks] = wstawiany;
  zajete++;
  if(double(zajete)/size() >= 0.7)
    rehash(1);
  
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
  DynamicArray<Slot>* tablica = (indeks >= size()/2 ? &tab2 : &tab1);
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

  DynamicArray<Slot>* tablica = (indeks >= size()/2 ? &tab2 : &tab1);
  return (*tablica)[indeks%(size()/2)].get_key();
}

//zgodnie z przyjetym zalozeniem, jesli rozmiar to np 200, to indeksy 0-99 sa w tab1, a 100-199 w tab2
size_t CuckooStrategy::search(const wchar_t* klucz) {
  size_t index = hash_fun1(klucz, size()/2, seed[0]); //najpierw szukamy w 1
  if(std::wcscmp(klucz, L"ROMAN") == 0) {
     std::cout << "Szukam Romana!" << std::endl;
     std::cout << "szukam, indeksy: " << hash_fun1(klucz, size()/2, seed[0]) << " " << hash_fun2(klucz, size()/2, seed[1]) << std::endl; 
  }
  //std::cout << "szukam, indeksy: " << hash_fun1(klucz, size()/2, seed[0]) << " " << hash_fun2(klucz, size()/2, seed[1]) << std::endl; 
  if(tab1[index].zajete() && (wcscmp(tab1[index].get_val(), klucz) == 0))
    return index;

  index = hash_fun2(klucz, size()/2, seed[1]); //potem szukamy w 2s
  if(tab2[index].zajete() && (wcscmp(tab2[index].get_val(), klucz) == 0))
    return tab1.get_size() + index;

  throw std::out_of_range("Nie ma takiego klucza w zbiorze!");
}

int CuckooStrategy::generate_key(const wchar_t* klucz) {
  return hash_fun1(klucz, size()/2, seed[0]);
}

void CuckooStrategy::_show() const { tab1._show(); tab2._show(); }

size_t CuckooStrategy::size() const { return 2*tab1.get_size(); /*oba maja miec taki sam*/ };

#endif
