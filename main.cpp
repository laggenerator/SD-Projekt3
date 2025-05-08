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
  mapa.insert(420, L"ZAŻÓŁĆ GĘŚLĄ JAŹŃ");
  mapa.insert(420, L"secs");

  // mapa._show();
  int elo = mapa.search(L"EUSTACHY");
  std::cout << "POZYCJA: " << elo << std::endl;
  std::cout << "JEST " << mapa.get_val(L"EUSTACHY") << " EUSTACHÓW" << std::endl;
  std::cout << "JEST " << mapa.get_val(L"sigma") << " secsów" << std::endl;
  return 0;
}
