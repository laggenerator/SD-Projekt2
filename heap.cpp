#include "heap.hh"
#include <iostream>

void Heap::heapify_up(size_t i) {
  while (i > 0) {
    size_t rodzic = (i - 1) / 2;
    if (dane[i] <= dane[rodzic]) {
      break; //własność kopca przywrócona
    }
    Pair mem = dane[i];
    dane[i] = dane[rodzic];
    dane[rodzic] = mem;
    i = rodzic;
  }
}

void Heap::heapify_down(size_t i) {
  size_t rodzic = i;
  size_t l = 2 * i + 1; //lewe dziecko = 2*i + 1
  size_t p = 2 * i + 2; //prawe = 2*i + 2
 
  if (l < dane.size() && dane[l] > dane[rodzic])
    rodzic = l;
 
  if (p < dane.size() && dane[p] > dane[rodzic])
    rodzic = p;
  //teraz najwiekszy z tych trzech jest faktycznie jako rodzic, tylko zamienic
    
  //jesli i to nie rodzic to jest zle
  if (rodzic != i) {
    Pair mem = dane[i];
    dane[i] = dane[rodzic];
    dane[rodzic] = mem;
    
    //rekurencyjnie, jakby nowe dziecko dodane na largest naruszalo poddrzewo ponizej
    heapify_down(rodzic);
  }
}

void Heap::insert(Pair val) {
  dane.push_back(val);
  heapify_up(dane.size()-1);
}

Pair Heap::extract_max() {
  Pair rezultat = dane[0];
  dane[0] = dane[dane.size()-1];
  dane[dane.size()-1] = rezultat;
  dane.remove_back();
  if(dane.size() > 0)
    heapify_down(0);

  return rezultat;
}

Pair& Heap::find_max() {
  return dane[0];
}

Pair* Heap::find(const char* val) {
  if(val == nullptr) return nullptr;
  
  for(size_t i = 0; i < dane.size(); ++i) {
    if (strncmp(dane[i].get_val(), val, VAL_SIZE) == 0) {
      return &dane[i];
    }
  }
  
  return nullptr;
}

void Heap::decrease_key(const char* val, int i) {
  Pair* zmieniany = find(val);
  if(zmieniany != nullptr)
    zmieniany->set_key(zmieniany->get_key()-i);
}

void Heap::increase_key(const char* val, int i) {
  decrease_key(val, -i);
}

void Heap::modify_key(const char* val, int k) {
  Pair* zmieniany = find(val);
  if(zmieniany != nullptr)
    zmieniany->set_key(k);
}

void Heap::build(DynamicArray& tablica) {
  dane = tablica;
  //od ostatniego nie-liścia do korzenia
  for (int i = dane.size()/2 - 1; i >= 0; --i) {
    heapify_down(i);
  }
}

void Heap::build(std::initializer_list<Pair> lista) {
  dane = DynamicArray(); //czyszczonko

  //do danych
  for (const Pair& para : lista) {
    dane.push_back(para);
  }

  //kopcowanie
  for (int i = dane.size() / 2 - 1; i >= 0; --i) {
    heapify_down(i);
  }
}
