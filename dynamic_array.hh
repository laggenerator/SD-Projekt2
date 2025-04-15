#ifndef DYN_ARR_H
#define DYN_ARR_H

#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <cstring>
#include <cstdlib>
#include "pair.hh"

class DynamicArray {
private:
  Pair *dane;
  std::size_t rozmiar;
  std::size_t pojemnosc;
    
  void resize(std::size_t nowa_pojemnosc);
  void shrink();
    
public:
  DynamicArray();
  ~DynamicArray();
    
  void push_back(Pair wartosc);
  void push_front(Pair wartosc);
  void push_at(unsigned int n, Pair wartosc);
      
  Pair remove_back();
  Pair remove_front();
  Pair remove_at(unsigned int n);
      
  int find(Pair wartosc); //szuka po kluuuuczu i zwraca indeks
  Pair at_position(unsigned int n); 

  size_t size() { return rozmiar; }
  size_t capacity() { return pojemnosc; }
    
  void _show();

  Pair& operator[](size_t i) {
    if(i >= rozmiar) throw std::out_of_range("Indeks poza zakresem tablicy!");
    return dane[i];
  }
  DynamicArray& operator=(const DynamicArray& other);
};
    

#endif
