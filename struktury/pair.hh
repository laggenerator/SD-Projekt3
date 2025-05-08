#ifndef PAIR_H
#define PAIR_H
#include <cstring>
#include <ostream>

#define VAL_SIZE 21

class Pair {
  int key;
  wchar_t val[VAL_SIZE];
public:
  Pair(int k, const wchar_t* v) : key(k) {
    wcsncpy(val, v, VAL_SIZE-1);
    val[VAL_SIZE-1] = L'\0';
  };
  Pair(const Pair& p) : key(p.key) {
    wcsncpy(val, p.val, VAL_SIZE-1);
    val[VAL_SIZE-1] = L'\0';
  };
  Pair() : key(0) { memset(val, 0, VAL_SIZE*sizeof(wchar_t)); }

  
  int get_key() const { return key; }
  const wchar_t* get_val() const { return val; }
  void set_key(int k) { key = k; }
  void set_val(const wchar_t* v) { wcsncpy(val, v, VAL_SIZE-1); val[VAL_SIZE-1] = '\0'; }

  //operatory dzialające na wartościach klucza
  bool operator==(const Pair& p) const {
    return (std::wcsncmp(val, p.val, VAL_SIZE) == 0);
  }
  bool operator!=(const Pair& p) const {
    return !(*this == p);
  }bool operator<(const Pair& p) const {
    return std::wcscmp(val, p.val) < 0;
  }
  
  bool operator>(const Pair& p) const {
    return std::wcscmp(val, p.val) > 0;
  }
  
  bool operator<=(const Pair& p) const {
    return std::wcscmp(val, p.val) <= 0;
  }
  
  bool operator>=(const Pair& p) const {
    return std::wcscmp(val, p.val) >= 0;
  }  
  Pair& operator=(const Pair& p) {
    if (this != &p) {
      key = p.key;
      wcsncpy(val, p.val, VAL_SIZE-1);
      val[VAL_SIZE-1] = L'\0';
    }
    return *this;
  }

  friend std::ostream& operator<<(std::ostream& os, const Pair& p);
};

inline std::ostream& operator<<(std::ostream& os, const Pair& p) {
  //val to łancuch wcharów, nie wyswietli go bo kodowanie bla bla, jest tez szeroki i cout nie zrozumie wiec
  //uzywa sie std::wcstombs zeby przerobic go na chary, ktore juz sie wyswietli (utf-8), utf-8 maks 4 bajty, wiec rozmiar*4
  char mbstr[VAL_SIZE*4];
  std::wcstombs(mbstr, p.val, VAL_SIZE*4);
  os << "(" << p.key << "|" << mbstr << ")";
  return os;
}

#endif
