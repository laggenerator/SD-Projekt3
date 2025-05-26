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
  
  testInsertOptymistycznyChaining(std::make_unique<LinkStrategy>(fnv_1, arraysize/0.69), &ludzie, 2137, czasyOPT[0]);
  testInsertOptymistycznyLinear(std::make_unique<LinearStrategy>(fnv_1, arraysize/0.69), &ludzie, 2137, czasyOPT[1]);


  testInsertPesymistycznyChaining(std::make_unique<LinkStrategy>(fnv_1), &ludzie, 2137, czasyPES[0]);
  testInsertPesymistycznyLinear(std::make_unique<LinearStrategy>(fnv_1), &ludzie, 2137, czasyPES[1]);


  zapisz("insertOPT.csv", czasyOPT);
  zapisz("insertPES.csv", czasyPES);
}