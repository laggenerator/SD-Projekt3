/* DODAWANIE:
OPTYMISTYCZNY:
  Separate chaining: wywalone
  Linear: kubełek jest pusty
PESYMISTYCZNY:
  Separate chaining: wywalone
  Linear: kubełek 70%*pojemnosc-1 jest wolny dopiero
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
void testInsertSredni(std::unique_ptr<HashMapStrategy> hashmap, DynamicArray<Pair> *dane, int seed, double* czasy){
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
    // PESYMISTYCZNIE CAŁY CZAS WKŁADA SIĘ TEN SAM KLUCZ CZYLI LINEAR MUSI INSERTOWAĆ DALEJ I DALEJ OD KLUCZA
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

/* USUWANIE:
OPTYMISTYCZNY:
  Separate chaining: na początku/końcu kubełka
  Linear: jest w swoim kubełku
PESYMISTYCZNY:
  Separate chaining: środek kubełka trzeba przejść n-elementów
  Linear: jest w kubełku najdalej od swojego (70%*pojemnosc -1 )%pojemnosc
*/

void testRemoveOptymistycznyChaining(std::unique_ptr<LinkStrategy> hashmap, DynamicArray<Pair> *dane, int seed, double* czasy){
  const size_t rozmiar = dane->get_size();
  for(size_t i=0;i<rozmiar;i++){
    czasy[i]=0;
  }
  for(int proba=0;proba<ILOSC_PROBEK;proba++){
    for(size_t i=0;i<rozmiar;i++){
      // std::cout << "elo " << i  << std::endl;
      hashmap->insert((*dane)[i]);
    }
    for(size_t i=rozmiar-1;i>0;i--){
      // Może oszukane ale optymistyczne nie
      int klucz = hashmap->generate_key((*dane)[i].get_val());
      int n = hashmap->dane[klucz].find_index((*dane)[i]);
      Pair para = hashmap->dane[klucz].remove_at(n);
      hashmap->dane[klucz].push_front(para);

      auto start = std::chrono::high_resolution_clock::now();
      hashmap->remove((*dane)[i].get_val());
      auto end = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double, std::nano> czas = end - start;
      czasy[i] += czas.count();
    }
  }
  for(size_t i=0;i<rozmiar;i++){
    czasy[i] /= ILOSC_PROBEK;
  }
}

void testRemovePesymistycznyChaining(std::unique_ptr<LinkStrategy> hashmap, DynamicArray<Pair> *dane, int seed, double* czasy){
  const size_t rozmiar = dane->get_size();
  for(size_t i=0;i<rozmiar;i++){
    czasy[i]=0;
  }
  for(int proba=0;proba<ILOSC_PROBEK;proba++){
    for(size_t i=0;i<rozmiar;i++){
      hashmap->insert((*dane)[i]);
    }
    for(size_t i=rozmiar-1;i>0;i--){
      // Może oszukane ale pesymistyczne nie
      int klucz = hashmap->generate_key((*dane)[i].get_val());
      int n = hashmap->dane[klucz].find_index((*dane)[i]);
      Pair para = hashmap->dane[klucz].remove_at(n);
      int srodek = hashmap->dane[klucz].get_size() / 2;
      hashmap->dane[klucz].push_at(srodek, para);

      auto start = std::chrono::high_resolution_clock::now();
      hashmap->remove((*dane)[i].get_val());
      auto end = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double, std::nano> czas = end - start;
      czasy[i] += czas.count();
    }
  }
  for(size_t i=0;i<rozmiar;i++){
    czasy[i] /= ILOSC_PROBEK;
  }
}



void testRemoveOptymistycznyLinear(std::unique_ptr<LinearStrategy> hashmap, DynamicArray<Pair> *dane, int seed, double* czasy){
  const size_t rozmiar = dane->get_size();
  srand(time(NULL));
  for(size_t i=0;i<rozmiar;i++){
    czasy[i]=0;
  }
  for(int proba=0;proba<ILOSC_PROBEK;proba++){
    for(size_t i=0;i<rozmiar;i++){
      hashmap->insert((*dane)[i]);
      // std::cout << "Dodawanie" << std::endl;
    }
    // OPTYMISTYCZNIE ZAWSZE JEST WSZYSTKO NA SWOIM MIEJSCU CO POWINNO BYC MOZLIWE PO PROSTU WKLADAJAC WSZYSTKO BO MOZE NIE BEDZIE KOLIZJI MAM NADZIEJE
    // Czasem tu sie zdarza kolizja ale zawsze wychodzi jedynie przemieszczenie o 1 element w prawo co da stały nakład niezauważalny praktycznie w wynikach
    for(size_t i=rozmiar-1;i>0;i--){
      bool udaloSie = false;
      auto start = std::chrono::high_resolution_clock::now();
      udaloSie = hashmap->remove((*dane)[i].get_val());
      auto end = std::chrono::high_resolution_clock::now();
      // std::cout << "usuwanie" << std::endl;
      std::chrono::duration<double, std::nano> czas = end - start;
      if(udaloSie){
        czasy[i] += czas.count();
      } else {
        std::cout << "Porazka" << std::endl;
        czasy[i] = -1000;
      }
    }
  }
  for(size_t i=0;i<rozmiar;i++){
    czasy[i] /= ILOSC_PROBEK;
  }
}
void testRemovePesymistycznyLinear(std::unique_ptr<LinearStrategy> hashmap, DynamicArray<Pair> *dane, int seed, double* czasy){
  const size_t rozmiar = dane->get_size();
  srand(time(NULL));
  for(size_t i=0;i<rozmiar;i++){
    czasy[i]=0;
  }
  for(int proba=0;proba<ILOSC_PROBEK;proba++){
    for(size_t i=rozmiar-1;i>4;i/=2){
      int dodane = 0;
      std::cout << "Ilosc danych(slotów): " << i << "(" << i/0.69 << ")" << std::endl;
      auto hash_func = hashmap->get_hash_function();
      auto dummyMap = std::make_unique<LinearStrategy>(hash_func, i/0.69);
      int rozmiarDummy = dummyMap->size();
      for(size_t j=0;j<i;j++){
        int og_klucz = dummyMap->generate_key((*dane)[j].get_val());
        int klucz = (og_klucz + rozmiarDummy + i) % rozmiarDummy;
        // std::cout << "dupa1 " << og_klucz << " = " << klucz << " : " << rozmiarDummy << std::endl;
        while (dummyMap->dane[klucz].zajete() && !dummyMap->dane[klucz].usuniete()) {
          klucz = (klucz + 1) % rozmiarDummy;
          // std::cout << (klucz+1)%rozmiarDummy << " -> " << klucz << " : " << rozmiarDummy << std::endl;
        }
        // std::cout << "dupa2" << std::endl;
        dummyMap->dane[klucz].ustaw((*dane)[j].get_key(), (*dane)[j].get_val());
        // std::cout << "dupa3 " << j << " : " << i << std::endl;
      }
      std::cout << "Dodano elementy" << std::endl;
      for(size_t j=i-1;j>i/2;j--){
        // auto imie = (*dane)[j].get_val();
        // std::cout << imie << " - " << dummyMap->generate_key(imie) << " : " << dummyMap->size() << std::endl;
        auto start = std::chrono::high_resolution_clock::now();
        dummyMap->remove((*dane)[j].get_val());
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::nano> czas = end - start;
        czasy[j] += czas.count();
      }
      std::cout << "Usunieto elementy" << std::endl;
    }
  }
  for(size_t i=0;i<rozmiar;i++){
    czasy[i] /= ILOSC_PROBEK;
  }
}

void testRemoveSredni(std::unique_ptr<HashMapStrategy> hashmap, DynamicArray<Pair> *dane, int seed, double* czasy){
  const size_t rozmiar = dane->get_size();
  for(size_t i=0;i<rozmiar;i++){
    czasy[i]=0;
  }
  for(int proba=0;proba<ILOSC_PROBEK;proba++){
    for(size_t i=0;i<rozmiar;i++){
      hashmap->insert((*dane)[i]);
    }
    for(size_t i=rozmiar-1;i>0;i--){
      auto start = std::chrono::high_resolution_clock::now();
      hashmap->remove((*dane)[i].get_val());
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