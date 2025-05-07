#include <iostream>
#include <algorithm>
#include <fstream>
#include <cmath>
#include <locale>
#include "./struktury/hash.hh"
#include "testy.hh"

int main() {
  std::setlocale(LC_ALL, "en_US.utf8"); //dzialaaaaa dziala dziala dziala
  LinkStrategy mapa(hash1);
  mapa.insert(120, L"KAŹMIERZ");
  mapa.insert(120, L"EUSTACHY");
  mapa.insert(120, L"ZAŻÓŁĆ GĘŚLĄ JAŹŃ");
  mapa._show();
  return 0;
}
