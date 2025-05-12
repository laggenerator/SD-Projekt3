#include <iostream>
#include <algorithm>
#include <fstream>
#include <cmath>
#include <locale>
#include "./struktury/hash.hh"
#include "testy.hh"
#include "wczytaj.hh"

int main() {  
  DynamicArray<Pair> ludzie = wczytajCSV("./dane/meskie.csv");
  DynamicArray<Pair> kobiety = wczytajCSV("./dane/zenskie.csv");

  size_t arraysize = kobiety.get_size();
  for(size_t i=0;i<arraysize;i++){
    ludzie.push_back(kobiety[i]);
  }
  arraysize = ludzie.get_size();
  std::cout << "Zmergowano tablice" << std::endl;
  std::cout << ludzie.at_position(0) << std::endl;
  std::cout << ludzie[0] << std::endl;
  std::setlocale(LC_ALL, "en_US.utf8"); //dzialaaaaa dziala dziala dziala
  LinkStrategy mapa(modulo_hash);
  for(size_t xxx=0;xxx<100;xxx++)
  mapa.insert(ludzie[xxx]);
  
  mapa._show();
  // ISTNIEJĄCE
  // std::cout << ludzie[0].get_val() << std::endl;
  int elo = mapa.search(ludzie[0].get_val());
  std::cout << "POZYCJA: " << elo << " klucz: " << modulo_hash(ludzie[0].get_val(), mapa.size()) << std::endl;
  int ilosc = mapa.get_val(ludzie[0].get_val());
  std::cout << "Jest " << ilosc << " ludzi o imieniu: " << ludzie[0] << std::endl;
  elo = mapa.search(ludzie[1].get_val());
  std::cout << "POZYCJA: " << elo << std::endl;
  ilosc = mapa.get_val(ludzie[1].get_val());
  std::cout << "Jest " << ilosc << " ludzi o imieniu: " << ludzie[1] << std::endl;
  
  // NIEISTNIEJĄCY
  try {
    elo = mapa.search(L"Arrur");
    std::cout << "POZYCJA: " << elo << std::endl;
  } catch (const std::out_of_range& err){
    std::cout << "Błąd: " << err.what() << std::endl;
  }
  try {
    ilosc = mapa.get_val(L"Arrur");
    std::cout << "JEST " << ilosc << " Arrurów" << std::endl;
  } catch (const std::out_of_range& err){
    std::cout << "Błąd: " << err.what() << std::endl;
  }
  return 0;
}
