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
  
  std::cout << "Optymistyczny insert chaining" << std::endl;
  testInsertOptymistycznyChaining(std::make_unique<LinkStrategy>(fnv_1, arraysize/0.69), &ludzie, 2137, czasyOPT[0]); // Optymistycznie O(1) - Wkłada tak czy inaczej do tego kubełka po prostu na tail
  std::cout << "Optymistyczny insert linear" << std::endl;
  testInsertOptymistycznyLinear(std::make_unique<LinearStrategy>(fnv_1, arraysize/0.69), &ludzie, 2137, czasyOPT[1]); // Optymistycznie O(1)

  std::cout << "Pesymistyczny insert chaining" << std::endl;
  testInsertPesymistycznyChaining(std::make_unique<LinkStrategy>(fnv_1), &ludzie, 2137, czasyPES[0]); // Pesymistycznie O(1) -- to samo co optymistycznie
  std::cout << "Pesymistyczny insert linear" << std::endl;
  testInsertPesymistycznyLinear(std::make_unique<LinearStrategy>(fnv_1), &ludzie, 2137, czasyPES[1]); // Pesymistycznie O(n) -- n elementów przesunięte od klucza
  
  std::cout << "Sredni insert chaining" << std::endl;
  testInsertSredni(std::make_unique<LinkStrategy>(fnv_1), &ludzie, 2137, czasyAVG[0]); // Średni to po prostu wkładanie danych i co wyjdzie to wyjdzie
  std::cout << "Sredni insert linear" << std::endl;
  testInsertSredni(std::make_unique<LinearStrategy>(fnv_1), &ludzie, 2137, czasyAVG[1]); // Średni to po prostu wkładanie danych i co wyjdzie to wyjdzie
  std::cout << "Sredni insert cuckoo" << std::endl;
  testInsertSredni(std::make_unique<CuckooStrategy>(fnv_1_seed, djb2_seed, 200000), &ludzie, 2137, czasyAVG[2]); // Średni to po prostu wkładanie danych i co wyjdzie to wyjdzie

  zapisz("insertOPT.csv", czasyOPT);
  zapisz("insertPES.csv", czasyPES);
  zapisz("insertAVG.csv", czasyAVG);


  std::cout << "Optymistyczny remove chaining" << std::endl;
  testRemoveOptymistycznyChaining(std::make_unique<LinkStrategy>(fnv_1, arraysize*2), &ludzie, 2137, czasyOPT[0]); // Optymistycznie O(1) - Wykłada z pierwszej pozycji
  std::cout << "Optymistyczny remove linear" << std::endl;
  testRemoveOptymistycznyLinear(std::make_unique<LinearStrategy>(fnv_1, arraysize*2), &ludzie, 2137, czasyOPT[1]); // Optymistycznie O(1) - Wykłada ze swojego kubełka
  std::cout << "Pesymistyczny remove chaining" << std::endl;
  testRemovePesymistycznyChaining(std::make_unique<LinkStrategy>(fnv_1), &ludzie, 2137, czasyPES[0]); // Pesymistycznie O(n w kubelku) w praktyce O(1) -- Wyklada ze srodka kubelka
  std::cout << "Pesymistyczny remove linear" << std::endl;
  testRemovePesymistycznyLinear(std::make_unique<LinearStrategy>(fnv_1), &ludzie, 2137, czasyPES[1]); // Pesymistycznie O(n) -- Jedzie n elementów bo wartosc jest na (klucz - 1) % pojemnosc

  std::cout << "Sredni remove chaining" << std::endl;
  testRemoveSredni(std::make_unique<LinkStrategy>(fnv_1), &ludzie, 2137, czasyAVG[0]);
  std::cout << "Sredni remove linear" << std::endl;
  testRemoveSredni(std::make_unique<LinearStrategy>(fnv_1), &ludzie, 2137, czasyAVG[1]);
  std::cout << "Sredni remove cuckoo" << std::endl;
  testRemoveSredni(std::make_unique<CuckooStrategy>(fnv_1_seed, djb2_seed, 200000), &ludzie, 2137, czasyAVG[2]);

  zapisz("removeOPT.csv", czasyOPT);
  zapisz("removePES.csv", czasyPES);
  zapisz("removeAVG.csv", czasyAVG);
}
