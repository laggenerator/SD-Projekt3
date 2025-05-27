#ifndef WCZYTAJ_HH
#define WCZYTAJ_HH

#include <fstream>
#include <sstream>
#include <locale>
#include <codecvt>
#include <string>
#include <cwchar>  // wcsncpy
#include "unordered_map"

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


DynamicArray<Pair> mergeTablic(DynamicArray<Pair>& arr1, DynamicArray<Pair>& arr2){
  std::unordered_map<std::wstring, int> imiona;

  for(size_t i=0;i<arr1.get_size();i++){
    Pair p = arr1[i];
    std::wstring imie = p.get_val();
    int wartosc = p.get_key();

    if(imiona.find(imie) == imiona.end() || wartosc > imiona[imie]){
      imiona[imie] = wartosc;
    }
  }

  for(size_t i=0;i<arr2.get_size();i++){
    Pair p = arr2[i];
    std::wstring imie = p.get_val();
    int wartosc = p.get_key();

    if(imiona.find(imie) == imiona.end() || wartosc > imiona[imie]){
      imiona[imie] = wartosc;
    }
  }

    std::vector<std::pair<int, std::wstring>> temp;
    for (const auto& para : imiona) {
        temp.emplace_back(para.second, para.first);
    }

    std::sort(temp.begin(), temp.end(), [](const auto& a, const auto& b) {
        return a.first > b.first;
    });

    DynamicArray<Pair> merged;
    for (const auto& entry : temp) {
        merged.push_back(Pair(entry.first, entry.second.c_str()));
    }

    return merged;
}

#define N_ZAPIS 500
#define ROZMIAR_TESTU /*65924*/ 45000

void zapisz(const char* nazwa_pliku, double dane[3][ROZMIAR_TESTU], bool testfunkcji = false) {
  //zapis do pliku
  std::ofstream plik;
  plik.open(nazwa_pliku, std::ios::out | std::ios::trunc);
  if (!plik.is_open()) {
    std::cerr << "Nie można otworzyć pliku: " << nazwa_pliku << std::endl;
  }
  if(!testfunkcji){
    plik << "Rozmiar;Chaining;Linear;Cuckoo" << std::endl;
  } else {
    plik << "Rozmiar;FNV1;DJB2;Modulo" << std::endl;
  }
  //ogolnie pierwsza kolumna to po prostu i, ale np w usuwaniu to musi byc i+1, w insert juz i
  for(size_t i = 1; i < ROZMIAR_TESTU; i+=N_ZAPIS) {
    plik << i;
    for(size_t j = 0; j < 3; ++j) {
      plik << ";" << dane[j][i];
    }
    plik << std::endl;
  }
  plik.close();
}
#endif
