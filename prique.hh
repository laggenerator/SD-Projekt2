#ifndef HEAP_PRIQUE_HH
#define HEAP_PRIQUE_HH
#include <memory>
#include "pair.hh"
#include "heap.hh"
#include "dynamic_array.hh"
#include "list.hh"

//klasa wirtualna, po ktorej dziedziczy stategia kopca, listy, tablicy
//nastepnie dawana jako argument do konstruktora PriorityQueue
class PriorityQueueStrategy {
public:
  virtual void insert(int p, const char* val) = 0;
  virtual void insert(Pair p) = 0;

  virtual Pair extract_max() = 0;
  virtual Pair find_max() = 0;

  virtual void modify_key(const char* val, int z) = 0;
  virtual ~PriorityQueueStrategy() = default;

  virtual void _show() = 0;
};

class HeapStrategy : public PriorityQueueStrategy {
private:
  Heap dane;

public:
  void insert(int p, const char* val) override { dane.insert(Pair(p, val)); };
  void insert(Pair p) override { dane.insert(p); };

  Pair extract_max() override { return dane.extract_max(); };
  Pair find_max() override { return dane.find_max(); };

  void modify_key(const char* val, int z) override { dane.modify_key(val, z); };
  void _show() override { dane._show(); }
};

class ListStrategy : public PriorityQueueStrategy {
private:
  List dane;

public:
  //tutaj bedziemy od razu dodawac w dobrym miejscu i potem zdejmowac po prostu z pierwszego. PILNUJE KOLEJNOŚCI.
  void insert(int p, const char* val) override { insert(Pair(p, val)); };
  void insert(Pair p) override {
    if(dane.get_size() == 0)
      dane.push_front(p);
    else {
      Node* iter = dane.head;
      while(iter->val > p) {
	if(iter->next != nullptr)
	  iter = iter->next;
	else {
	  dane.push_back(p);
	  return;
	}
      }

      //jestesmy na pozycji (przegladany) na ktorej trzeba wstawic p, jesli byly z identycznym priorytetem to dodaje jako pierwszy (bo >, nie >=)
      if(iter == dane.head) {
	dane.push_front(p);
      }
      else {
	Node* nowy = new Node(p, iter->prev, iter); 
	Node* temp = iter;
	iter = nowy;
	temp->prev = iter;
	iter->prev->next = iter;
      }
    }
  };

  //proste operacje bo dodawanie jest skomplikowane
  Pair extract_max() override { return dane.remove_front(); };
  Pair find_max() override { return dane.head->val;};

  //zmienia tylko pierwszemu, ktory sie napatoczy (jesli ma kazdemu to hahaha)
  //poza tym to dziala na zasadzie problemu teleportacji -- kasuje z tablicy (zabija) i dodaje nowy z taka sama wartosci ale nowym kluczem
  void modify_key(const char* val, int z) override {
    Node* iter = dane.head;
    //szuka pasującego
    while (iter && std::strcmp(iter->val.get_val(), val) != 0) {
        iter = iter->next;
    }
    if (!iter) return; //nie ma takiego w kolejce

    //wywalamy go z listy
    if (iter->prev) iter->prev->next = iter->next;
    else dane.head = iter->next;

    if (iter->next) iter->next->prev = iter->prev;
    else dane.tail = iter->prev;

    dane.size--;

    //usuwanie, zeby nie bylo ze wyciek pamieci
    char tmp_val[VAL_SIZE];
    strncpy(tmp_val, iter->val.get_val(), VAL_SIZE-1);
    tmp_val[VAL_SIZE-1] = '\0';
    delete iter;

    insert(z, tmp_val); //wstawia w dobrym miejscu
  };

  void _show() override { dane._show(); }
};

class Prique {
  std::unique_ptr<PriorityQueueStrategy> strategy; //wskaznik na klase bazowa zeby kazda dziedziczaca mogla byc
public:
  Prique(std::unique_ptr<PriorityQueueStrategy> s) : strategy(std::move(s)) {}

  void insert(int p, const char* val) { strategy->insert(Pair(p, val)); };
  void insert(Pair p) { strategy->insert(p); };

  Pair extract_max() { return strategy->extract_max(); };
  Pair find_max() { return strategy->find_max(); };

  void modify_key(const char* val, int z) { strategy->modify_key(val, z); };
  void _show() { strategy->_show(); }
};

#endif
