#ifndef LISTA_H
#define LISTA_H
#include "pair.hh"

template <typename T>
class List;
template <typename T>
std::ostream& operator<<(std::ostream&, const List<T>&);


template <typename T>
struct Node {
private:
  T val;
  Node<T>* prev;
  Node<T>* next;

public:
  Node(T v, Node<T>* p = nullptr, Node<T>* n = nullptr) : val(v), prev(p), next(n) {} // Konstruktor węzła

  T value() { return val; }

  template <typename> friend class List;
  
  friend std::ostream& operator<< <>(std::ostream&, const List<T>&);
};

// Lista dwukierunkowa
template <typename T>
class List {
  private:
  size_t size;
  Node<T>* head; // Wskaźnik na początek listy
  Node<T>* tail; // Wskaźnik na koniec listy
  
public:
  List() { head = nullptr; tail = nullptr; size = 0;}
  ~List() {
    // if(size > 0) std::cout << "Usuwam liste rozmiaru: " << size << " " << *this << std::endl;
    Node<T>* iter = head;
    while (iter != nullptr) {
      Node<T>* temp = iter;
      iter = iter->next;
      delete temp;
    }
  }

  void push_back(T val);
  void push_front(T val);
  void push_at(const size_t n, T val);

  T remove_back();
  T remove_front();
  T remove_at(size_t n);

  Node<T>* find(T val) const;
  size_t find_index(T val) const;
  Node<T>* at_position(size_t n) const;
  
  size_t get_size() const { return size; }
  void _show();

  friend std::ostream& operator<< <>(std::ostream&, const List<T>&);

  List& operator=(const List& other) {
    if (this == &other) return *this;
  
    // usuń bieżącą zawartość
    Node<T>* iter = head;
    while (iter != nullptr) {
      Node<T>* temp = iter;
      iter = iter->next;
      delete temp;
    }
    head = nullptr;
  
    // skopiuj z other
    Node<T>* current = other.head;
    while (current != nullptr) {
      push_back(current->value());
      current = current->next;
    }
  
    return *this;
  }
  
};

template <typename T>
inline std::ostream& operator<<(std::ostream& os, const List<T>& l) {
  Node<T>* iter = l.head;
  if(iter != nullptr) {
    do {
      os << iter->val << "->";
      iter = iter->next;
    } while(iter != nullptr);
    os << "/0";
  }
  else
    os << "/0";
  return os;
}

template<typename T>
void List<T>::push_back(T val) {
  Node<T>* newNode = new Node<T>(val, tail, nullptr);
  if (head == nullptr) { // Lista jest pusta
    head = newNode;
    tail = newNode;
  } else {
    tail->next = newNode;
    tail = newNode;
  }
  size++;
}

template<typename T>
void List<T>::push_front(T val) {
  Node<T>* newNode = new Node<T>(val, nullptr, head);
  if(head == nullptr) { //dodajemy pierwszy element
    tail = newNode;
  }
  else
    head->prev = newNode;
  head = newNode;
  size++;
}

template<typename T>
void List<T>::push_at(const size_t n, T val) {
  if(n <= size) { //indeksujemy oczywiscie od 0, wiec jeszcze jako n element mozna dodac, po prostu nowy na koncu
    if(n == 0)
      push_front(val);
    else if(n == size)
      push_back(val);
    else {
      Node<T>* iter;
      if(n < size/2) { //idziemy od head do przodu
	iter = head;
	for(size_t i = 0; i < n; ++i)
	  iter = iter->next;
      }
      else { //idziemy od tail do tylu
	iter = tail;
	for(size_t i = 0; i < size-n; ++i)
	  iter = iter->prev;
      }
      Node<T>* newNode = new Node<T>(val, iter, iter->next);
      iter->next->prev = newNode;
      iter->next = newNode;
      size++;
    }
  }
  else {
    throw std::invalid_argument("Indeks poza zakresem listy!");
  }
}

template<typename T>
T List<T>::remove_back() {
  if(size == 0)
    throw std::out_of_range("Nie ma czego usuwać!");

  T result;
  if(size == 1) {
    result = head->val;
    delete head;
    head = nullptr;
    tail = nullptr;
  }
  else {
    result = tail->val;
    Node<T>* temp = tail;
    tail = tail->prev;
    tail->next = nullptr;
    delete temp;
  }
  size--;
  return result;
}

template <typename T>
T List<T>::remove_front() {
  if (size == 0) {
    throw std::out_of_range("Nie ma czego usuwać!");
  }

  T result = head->val;
  Node<T>* temp = head;
  if (size == 1) { //jeden element, bedzie potem pusta
    head = nullptr;
    tail = nullptr;
  } else {
    head = head->next;
    head->prev = nullptr;
  }
  
  delete temp;
  size--;
  return result;
}

template <typename T>
T List<T>::remove_at(size_t n) {
  if(n >= size)
    throw std::invalid_argument("Indeks poza zakresem listy!");

  T result;
  if(n < size) { //indeksujemy oczywiscie od 0, wiec juz n nie mozna usunac: size = 3, 0 -> 1 -> 2 -> NULL
    if(n == 0)
      return remove_front();
    else if(n == size-1)
      return remove_back();
    else {
      Node<T>* iter;
      if(n < size/2) { //idziemy od head do przodu
	iter = head;
	for(size_t i = 0; i < n; ++i)
	  iter = iter->next;
      }
      else { //idziemy od tail do tylu
	iter = tail;
	for(size_t i = 0; i < size-n-1; ++i)
	  iter = iter->prev;
      }
      result = iter->val;
      iter->prev->next = iter->next;
      iter->next->prev = iter->prev;
      delete iter;
      size--;
    }
  }

  return result;
}

template <typename T>
Node<T>* List<T>::find(T val) const { //po prostu zwraca wskaznik na element jesli go znajdzie, inaczej nullptr
  if(size == 0)
    return nullptr;
    
  Node<T>* iter = head;
  while(iter != nullptr) {
    if(iter->val == val)
      return iter;
    iter = iter->next;
  }
  
  return nullptr;
}

template <typename T>
size_t List<T>::find_index(T val) const { //zwraca „indeks” elementu jesli go znajdzie, inaczej zwroci size
  if(size == 0)
    return size;
  Node<T>* iter = head;
  int i = 0;
  std::cout << iter->val << " = " << val << std::endl;
  while(iter->val != val) {
    iter = iter->next;
    ++i;
    if(iter == nullptr)
      return size;
  }
  
  return i;
}

template <typename T>
Node<T>* List<T>::at_position(size_t n) const {
  if(n >= size)
    throw std::out_of_range("Indeks jest poza zakresem!");

  Node<T>* iter = head;
  for(size_t i = 0; i < n; ++i)
    iter = iter->next;

  return iter;
}

template <typename T>
void List<T>::_show() {
  Node<T>* iter = head;
  if(iter != nullptr) {
    do {
      std::cout << iter->val << "->";
      iter = iter->next;
    } while(iter != nullptr);
    std::cout << "/0\n";
    std::cout << "head: " << head->val << " " << "tail: " << tail->val << std::endl;

    iter = tail;
    do {
      std::cout << iter->val << "->";
      iter = iter->prev;
    } while(iter != nullptr);
    std::cout << "/0\n";
    std::cout << "head: " << head->val << " " << "tail: " << tail->val << std::endl;
  }
  else
    std::cout << "Lista jest pusta!" << std::endl;
}



#endif
