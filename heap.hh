#ifndef HEAP_HH
#define HEAP_HH
#include "pair.hh"
#include "dynamic_array.hh"

//kopiec max (najwyzsza wartosc to RODZIC OSTATECZNY
class Heap {
private:
  DynamicArray dane;
  void heapify_up(size_t i);
  void heapify_down(size_t i);
public:
  Heap() {}
  //wygodne
  Heap(std::initializer_list<Pair> lista) { build(lista); }
  void insert(Pair val);
  Pair extract_max();
  Pair& find_max();
  Pair* find(const char* val);
  //z poniższymi mam ten problem, że nie wiem jaki argument maja przyjmowac, rudy nie mowi -- indeks czy wartosc, klucz?
  //delete
  void decrease_key(const char* val, int i = 1);
  void increase_key(const char* val, int i = 1);
  //chyba po wartosciach:
  /*modify-key() – wciąż koszt O(n) (konieczność znalezienia węzła).
▶ Możemy dodatkowo przechowywać słownik mapujący indeks na węzeł.
▶ Koszt operacji spada do O(log n), ale stała wzrasta.
▶ Słownik oznacza konieczność dodatkowej pamięci.*/
  void build(DynamicArray& tablica);
  void build(std::initializer_list<Pair> lista);
  size_t size() { return dane.size(); }

  void _show() { dane._show(); }
};

#endif
