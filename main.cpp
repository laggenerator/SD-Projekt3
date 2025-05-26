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
  DynamicArray<Pair> menszczyzni = wczytajCSV("./dane/meskie.csv");
  DynamicArray<Pair> kobiety = wczytajCSV("./dane/zenskie.csv");

  DynamicArray<Pair> ludzie = mergeTablic(menszczyzni, kobiety);
  size_t arraysize = ludzie.get_size();
  std::cout << "Zmergowano tablice" << std::endl;
  std::cout << ludzie.at_position(0) << std::endl;
  std::cout << ludzie[0] << std::endl;
  std::setlocale(LC_ALL, "en_US.utf8"); //dzialaaaaa dziala dziala dziala
  
  
  /*

 // HUMAN NOWY KOD
  //z tymi haszami dziala, nie zalecalbym dawac tu tego modulo bo daje tylko maly zakres i bedzie czesto sie zapetlac
  CuckooStrategy mapa(fnv_1_seed, djb2_seed, 100000);
  //LinkStrategy mapa(fnv_1);
  for(size_t xxx=0;xxx<ludzie.get_size();xxx++) {
    try {
      if(ludzie[xxx].get_key() == 165855)
	std::cout << "KAROL" << std::endl;
      mapa.insert(ludzie[xxx]);
    } catch(std::exception& e) {
      //std::cout << e.what() << " przy probie wstawienia wartosci " << ludzie[xxx] << std::endl;
    }

    //plik << xxx << ";" << fnv_1(ludzie[xxx].get_val(), 100000) << std::endl;
  }
  
  // mapa._show();
  // ISTNIEJĄCE
  // std::cout << ludzie[0].get_val() << std::endl;
  for(int i = 40; i < 45; ++i) {
    std::cout << "Poszukuje " << ludzie[i] << std::endl;
    std::cout << "POZYCJA: " << mapa.search(ludzie[i].get_val()) << std::endl;
    std::cout << "Jest " << mapa.get_val(ludzie[i].get_val()) << " ludzi o imieniu: " << ludzie[i] << std::endl;
  }
  // NIEISTNIEJĄCY
  try {
    size_t elo = mapa.search(L"Arrur");
    std::cout << "POZYCJA: " << elo << std::endl;
  } catch (const std::out_of_range& err){
    std::cout << "Błąd: " << err.what() << std::endl;
  }
  try {
    int ilosc = mapa.get_val(L"Arrur");
    std::cout << "JEST " << ilosc << " Arrurów" << std::endl;
  } catch (const std::out_of_range& err){
    std::cout << "Błąd: " << err.what() << std::endl;
  }

  */
  // KOD Z TESTAMI
 
  std::cout << arraysize << std::endl;
  double czasyOPT[3][65924]; //Chaining, Linear, Cuckoo
  double czasyAVG[3][65924]; //Chaining, Linear, Cuckoo
  double czasyPES[3][65924]; //Chaining, Linear, Cuckoo
  
  // testInsertOptymistycznyChaining(std::make_unique<LinkStrategy>(fnv_1, arraysize/0.69), &ludzie, 2137, czasyOPT[0]); // Optymistycznie O(1) - Wkłada tak czy inaczej do tego kubełka po prostu na tail
  // testInsertOptymistycznyLinear(std::make_unique<LinearStrategy>(fnv_1, arraysize/0.69), &ludzie, 2137, czasyOPT[1]); // Optymistycznie O(1)

  // testInsertPesymistycznyChaining(std::make_unique<LinkStrategy>(fnv_1), &ludzie, 2137, czasyPES[0]); // Pesymistycznie O(1) -- to samo co optymistycznie
  // testInsertPesymistycznyLinear(std::make_unique<LinearStrategy>(fnv_1), &ludzie, 2137, czasyPES[1]); // Pesymistycznie O(n) -- n elementów przesunięte od klucza

  // testInsertSredni(std::make_unique<LinkStrategy>(fnv_1), &ludzie, 2137, czasyAVG[0]); // Średni to po prostu wkładanie danych i co wyjdzie to wyjdzie
  // testInsertSredni(std::make_unique<LinearStrategy>(fnv_1), &ludzie, 2137, czasyAVG[1]); // Średni to po prostu wkładanie danych i co wyjdzie to wyjdzie
  // testInsertSredni(std::make_unique<CuckooStrategy>(fnv_1_seed, djb2_seed, 200000), &ludzie, 2137, czasyAVG[2]); // Średni to po prostu wkładanie danych i co wyjdzie to wyjdzie

  // zapisz("insertOPT.csv", czasyOPT);
  // zapisz("insertPES.csv", czasyPES);
  // zapisz("insertAVG.csv", czasyAVG);


  testRemoveOptymistycznyChaining(std::make_unique<LinkStrategy>(fnv_1, arraysize*2), &ludzie, 2137, czasyOPT[0]); // Optymistycznie O(1) - Wykłada z pierwszej pozycji
  testRemoveOptymistycznyLinear(std::make_unique<LinearStrategy>(fnv_1, arraysize*2), &ludzie, 2137, czasyOPT[1]); // Optymistycznie O(1) - Wykłada ze swojego kubełka
  // testRemovePesymistycznyChaining(std::make_unique<LinkStrategy>(fnv_1, arraysize/0.69), &ludzie, 2137, czasyPES[0]); // Pesymistycznie O(n w kubelku) -- Wyklada ze srodka kubelka
  // testRemovePesymistycznyLinear(std::make_unique<LinearStrategy>(fnv_1, arraysize/0.69), &ludzie, 2137, czasyPES[1]); // Pesymistycznie O(n) -- Jedzie n elementów bo wartosc jest na (klucz - 1) % pojemnosc

  zapisz("removeOPT.csv", czasyOPT);
  zapisz("removePES.csv", czasyPES);
  // zapisz("removeAVG.csv", czasyAVG);
}
