/* DODAWANIE:
OPTYMISTYCZNY:
  Separate chaining: wywalone
  Linear: kubełek jest pusty
PESYMISTYCZNY:
  Separate chaining: wywalone
  Linear: kubełek 70%*pojemnosc-1 jest wolny dopiero
*/
/* USUWANIE:
OPTYMISTYCZNY:
  Separate chaining: na początku/końcu kubełka
  Linear: jest w swoim kubełku
PESYMISTYCZNY:
  Separate chaining: środek kubełka trzeba przejść n-elementów
  Linear: jest w kubełku najdalej od swojego (70%*pojemnosc -1 )%pojemnosc
*/

#ifndef TESTY_HH
#define TESTY_HH

#include <random>
#include <chrono>
#include <memory>
#include "wczytaj.hh"
#include "./struktury/hash_table.hh"

#define ILOSC_PROBEK 1

void testInsertOptymistycznyChaining(std::unique_ptr<LinkStrategy> hashmap, DynamicArray<Pair> *dane, int seed, double* czasy){
  const size_t rozmiar = dane->get_size();
  for(size_t i=0;i<rozmiar;i++){
    czasy[i]=0;
  }
  for(int proba=0;proba<ILOSC_PROBEK;proba++){
    for(size_t i=0;i<rozmiar;i++){
      auto start = std::chrono::high_resolution_clock::now();
      hashmap->insert((*dane)[i]);
      auto end = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double, std::nano> czas = end - start;
      czasy[i] += czas.count();
    }
  }
  for(size_t i=0;i<rozmiar;i++){
    czasy[i] /= ILOSC_PROBEK;
  }
}
void testInsertPesymistycznyChaining(std::unique_ptr<LinkStrategy> hashmap, DynamicArray<Pair> *dane, int seed, double* czasy){
  const size_t rozmiar = dane->get_size();
  for(size_t i=0;i<rozmiar;i++){
    czasy[i]=0;
  }
  for(int proba=0;proba<ILOSC_PROBEK;proba++){
    for(size_t i=0;i<rozmiar;i++){
      auto start = std::chrono::high_resolution_clock::now();
      hashmap->insert((*dane)[i]);
      auto end = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double, std::nano> czas = end - start;
      czasy[i] += czas.count();
    }
  }
  for(size_t i=0;i<rozmiar;i++){
    czasy[i] /= ILOSC_PROBEK;
  }

}
void testInsertOptymistycznyLinear(std::unique_ptr<LinearStrategy> hashmap, DynamicArray<Pair> *dane, int seed, double* czasy){
  const size_t rozmiar = dane->get_size();
  for(size_t i=0;i<rozmiar;i++){
    czasy[i]=0;
  }
  for(int proba=0;proba<ILOSC_PROBEK;proba++){
    srand(time(NULL));
    Pair kandydat = (*dane)[rand() % rozmiar];
    // OPTYMISTYCZNIE NIE MA REHASHOWANIA WIEC POJEMNOSC STARTOWA JEST RÓWNA ILOŚCI DANYCH (rozmiar tablicy imion)
    int klucz = hashmap->generate_key(kandydat.get_val());
    for(size_t i=0;i<rozmiar;i++){
      auto start = std::chrono::high_resolution_clock::now();
      hashmap->insert(kandydat);
      auto end = std::chrono::high_resolution_clock::now();
      if(!(hashmap->move(klucz, (klucz+i+1)%hashmap->size()))){
        throw std::runtime_error("Move w optymistycznym tescie linear nie zadzialal!");
      }
      std::chrono::duration<double, std::nano> czas = end - start;
      czasy[i] += czas.count();
    }
  }
  for(size_t i=0;i<rozmiar;i++){
    czasy[i] /= ILOSC_PROBEK;
  }
}
void testInsertPesymistycznyLinear(std::unique_ptr<LinearStrategy> hashmap, DynamicArray<Pair> *dane, int seed, double* czasy){
  const size_t rozmiar = dane->get_size();
  srand(time(NULL));
  for(size_t i=0;i<rozmiar;i++){
    czasy[i]=0;
  }
  for(int proba=0;proba<ILOSC_PROBEK;proba++){
    Pair kandydat = (*dane)[rand() % rozmiar];
    // OPTYMISTYCZNIE NIE MA REHASHOWANIA WIEC POJEMNOSC STARTOWA JEST RÓWNA ILOŚCI DANYCH (rozmiar tablicy imion)
    int klucz = hashmap->generate_key(kandydat.get_val());
    for(size_t i=0;i<rozmiar;i++){
      auto start = std::chrono::high_resolution_clock::now();
      hashmap->insert(kandydat);
      auto end = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double, std::nano> czas = end - start;
      czasy[i] += czas.count();
    }
  }
  for(size_t i=0;i<rozmiar;i++){
    czasy[i] /= ILOSC_PROBEK;
  }
}

#endif