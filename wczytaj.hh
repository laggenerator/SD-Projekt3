#ifndef WCZYTAJ_HH
#define WCZYTAJ_HH

#include <fstream>
#include <sstream>
#include <locale>
#include <codecvt>
#include <string>
#include <cwchar>  // wcsncpy

#include "./struktury/dynamic_array.hh"
#include "./struktury/pair.hh"
DynamicArray<Pair> wczytajCSV(const std::string& nazwa_pliku) {
  DynamicArray<Pair> wynik;

  std::setlocale(LC_ALL, "en_US.utf8");

  std::locale utf8_locale("en_US.UTF-8");
  std::wifstream plik(nazwa_pliku);
  plik.imbue(utf8_locale);

  if (!plik.is_open()) {
    std::cerr << "Błąd: Nie można otworzyć pliku: " << nazwa_pliku.c_str() << std::endl;
    return wynik;
  }

  std::wstring linia;
  if (!std::getline(plik, linia)) {
    std::cerr << "Błąd: Plik jest pusty." << std::endl;
    return wynik;
  }

  while (std::getline(plik, linia)) {
    std::wstringstream ss(linia);
    std::wstring imie, plec, liczba_wyst;

    std::getline(ss, imie, L',');
    std::getline(ss, plec, L',');
    std::getline(ss, liczba_wyst, L',');

    int liczba = std::stoi(liczba_wyst);
    Pair p(liczba, imie.c_str());
    wynik.push_back(p);
  }
  std::cout << "Przetworzono plik!" << std::endl;
  return wynik;
}
#endif