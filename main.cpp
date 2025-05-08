#include <iostream>
#include <algorithm>
#include <fstream>
#include <cmath>
#include <locale>
#include "./struktury/hash.hh"
#include "testy.hh"

int main() {
  std::setlocale(LC_ALL, "en_US.utf8"); //dzialaaaaa dziala dziala dziala
  LinkStrategy mapa(modulo_hash);
  mapa.insert(120, L"KAŹMIERZ");
  mapa.insert(69, L"EUSTACHY");
  mapa.insert(420, L"ESUTACHY");
  mapa.insert(120, L"secs");

  // ISTNIEJĄCE
  int elo = mapa.search(L"EUSTACHY");
  std::cout << "POZYCJA: " << elo << std::endl;
  int ilosc = mapa.get_val(L"EUSTACHY");
  std::cout << "JEST " << ilosc << " EUSTACHÓW" << std::endl;
  elo = mapa.search(L"ESUTACHY");
  std::cout << "POZYCJA: " << elo << std::endl;
  ilosc = mapa.get_val(L"ESUTACHY");
  std::cout << "JEST " << ilosc << " ESUTACHÓW" << std::endl;
  
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
