#include <iostream>
#include <algorithm>
#include <fstream>
#include <cmath>
#include <locale>
#include <fstream>
#include "./struktury/hash_table.hh"
#include "testy.hh"
#include "wczytaj.hh"

int main(){
  DynamicArray<Pair> ludzie = wczytajCSV("./dane/meskie.csv");
  DynamicArray<Pair> kobiety = wczytajCSV("./dane/zenskie.csv");

  size_t arraysize = kobiety.get_size();
  for(size_t i=0;i<arraysize;i++){
    ludzie.push_back(kobiety[i]);
  }
  arraysize = ludzie.get_size();
  std::setlocale(LC_ALL, "en_US.utf8"); //dzialaaaaa dziala dziala dziala
 
  std::cout << arraysize << std::endl;
  double czasyOPT[3][69180]; //Chaining, Linear, Cuckoo
  double czasyAVG[3][69180]; //Chaining, Linear, Cuckoo
  double czasyPES[3][69180]; //Chaining, Linear, Cuckoo
  
  testInsertOptymistycznyChaining(std::make_unique<LinkStrategy>(fnv_1, arraysize/0.69), &ludzie, 2137, czasyOPT[0]); // Optymistycznie O(1) - Wkłada tak czy inaczej do tego kubełka po prostu na tail
  testInsertOptymistycznyLinear(std::make_unique<LinearStrategy>(fnv_1, arraysize/0.69), &ludzie, 2137, czasyOPT[1]); // Optymistycznie O(1)

  testInsertPesymistycznyChaining(std::make_unique<LinkStrategy>(fnv_1), &ludzie, 2137, czasyPES[0]); // Pesymistycznie O(1) -- to samo co optymistycznie
  testInsertPesymistycznyLinear(std::make_unique<LinearStrategy>(fnv_1), &ludzie, 2137, czasyPES[1]); // Pesymistycznie O(n) -- n elementów przesunięte od klucza

  testInsertSredni(std::make_unique<LinkStrategy>(fnv_1), &ludzie, 2137, czasyAVG[0]); // Średni to po prostu wkładanie danych i co wyjdzie to wyjdzie
  testInsertSredni(std::make_unique<LinearStrategy>(fnv_1), &ludzie, 2137, czasyAVG[1]); // Średni to po prostu wkładanie danych i co wyjdzie to wyjdzie
  testInsertSredni(std::make_unique<CuckooStrategy>(fnv_1, djb2, arraysize*2), &ludzie, 2137, czasyAVG[2]); // Średni to po prostu wkładanie danych i co wyjdzie to wyjdzie

  zapisz("insertOPT.csv", czasyOPT);
  zapisz("insertPES.csv", czasyPES);
  zapisz("insertAVG.csv", czasyAVG);
}