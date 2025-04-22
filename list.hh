#ifndef LISTA_H
#define LISTA_H
#include "pair.hh"

struct Node {
private:
  Pair val;
  Node* prev;
  Node* next;

public:
  Node(Pair v, Node* p = nullptr, Node* n = nullptr) : val(v), prev(p), next(n) {} // Konstruktor węzła

  Pair value() { return val; }
  
  friend class List;
  friend class ListStrategy;
};

// Lista dwukierunkowa
class List {
  private:
  size_t size;
  Node* head; // Wskaźnik na początek listy
  Node* tail; // Wskaźnik na koniec listy
  
  public:
  List();
  ~List(); 

  void push_back(Pair val);
  void push_front(Pair val);
  void push_at(const size_t n, Pair val);

  Pair remove_back();
  Pair remove_front();
  Pair remove_at(size_t n);

  Node* find(Pair val) const; //po prostu zwraca wskaznik na element jesli go znajdzie, inaczej nullptr
  size_t find_index(Pair val) const; //zwraca „indeks” elementu jesli go znajdzie, inaczej zwroci size
  Node* at_position(size_t n) const;
  size_t get_size() { return size; }

  void _show();

  friend class ListStrategy;
};

#endif
