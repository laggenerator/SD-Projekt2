#ifndef PRIQUE_HH
#define PRIQUE_HH
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
  virtual size_t size() = 0;
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
  size_t size() override { return dane.size(); }
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

	if (iter->prev)
	  iter->prev->next = nowy;
	iter->prev = nowy;

	// Jeśli iter był headem, aktualizujemy head:
	if (iter == dane.head)
	  dane.head = nowy;

	dane.size++;
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
  size_t size() override { return dane.get_size(); }
};

class DescendArrayStrategy : public PriorityQueueStrategy {
  private:
    DynamicArray dane;

    // Funkcja do znajdywania miejsca na nową parę
    size_t znajdzPozycje(int klucz){
      size_t l = 0;
      size_t p = dane.size();

      while(l < p){
        size_t mid = l + (p - l)/2;
        if(dane.at_position(mid).get_key() <= klucz){
          l = mid + 1;
        } else {
          p = mid;
        }
      }
      return l;
    }

  public: 
  void insert(int p, const char* val) override { insert(Pair(p, val)); };
  void insert(Pair p) override {
    // Znajdujemy pozycje dla klucza w zbiorze malejącym nie wiem jak to opisac od najwiekszego do najmniejszego
    size_t poz = znajdzPozycje(p.get_key());
    dane.push_at(poz, p);
  }

  Pair extract_max() override {
    if(dane.size() == 0){
      throw std::out_of_range("Kolejka jest pusta");
    }
    return dane.remove_front(); // Sortujemy dodając, więc największy klucz jest pierwszy ;)
  }

  Pair find_max() override {
    if(dane.size() == 0){
      throw std::out_of_range("Kolejka jest pusta");
    }
    return dane.at_position(0); // Analogicznie jak wyżej
  }

  void modify_key(const char* val, int nowy_klucz) override {
    size_t i = 0;
    while(i < dane.size()){
      if(strcmp(dane[i].get_val(), val) == 0){
        Pair stary = dane[i];
        dane.remove_at(i);
        insert(Pair(nowy_klucz, stary.get_val()));
        return;
      }
      i++;
    }
    throw std::out_of_range("Wartość nie została odnaleziona w kolejce");
  }

  void _show() override {
    dane._show();
  }

  size_t size() override { return dane.size(); }
};

class AscendArrayStrategy : public PriorityQueueStrategy {
  private:
    DynamicArray dane;

    // Funkcja do znajdywania miejsca na nową parę
    size_t znajdzPozycje(int klucz){
      size_t l = 0;
      size_t p = dane.size();

      while(l < p){
        size_t mid = l + (p - l)/2;
        if(dane.at_position(mid).get_key() >= klucz){
          l = mid + 1;
        } else {
          p = mid;
        }
      }
      return l;
    }

  public: 
  void insert(int p, const char* val) override { insert(Pair(p, val)); };
  void insert(Pair p) override {
    // Znajdujemy pozycje dla klucza w zbiorze malejącym nie wiem jak to opisac od najwiekszego do najmniejszego
    size_t poz = znajdzPozycje(p.get_key());
    dane.push_at(poz, p);
  }

  Pair extract_max() override {
    if(dane.size() == 0){
      throw std::out_of_range("Kolejka jest pusta");
    }
    return dane.remove_back(); // Sortujemy dodając, więc największy klucz jest pierwszy ;)
  }

  Pair find_max() override {
    if(dane.size() == 0){
      throw std::out_of_range("Kolejka jest pusta");
    }
    return dane.at_position(dane.size() - 1); // Analogicznie jak wyżej
  }

  void modify_key(const char* val, int nowy_klucz) override {
    size_t i = 0;
    while(i < dane.size()){
      if(strcmp(dane[i].get_val(), val) == 0){
        Pair stary = dane[i];
        if(dane[i].get_key() == nowy_klucz){
          return;
        }
        dane.remove_at(i);
        insert(Pair(nowy_klucz, stary.get_val()));
        return;
      }
      i++;
    }
    throw std::out_of_range("Wartość nie została odnaleziona w kolejce");
  }

  void _show() override {
    dane._show();
  }

  size_t size() override { return dane.size(); }
};

class Prique {
  std::unique_ptr<PriorityQueueStrategy> strategy; //wskaznik na klase bazowa zeby kazda dziedziczaca mogla byc
public:
  Prique(std::unique_ptr<PriorityQueueStrategy>& s) : strategy(std::move(s)) {}

  void insert(int p, const char* val) { strategy->insert(Pair(p, val)); };
  void insert(Pair p) { strategy->insert(p); };

  Pair extract_max() { return strategy->extract_max(); };
  Pair find_max() { return strategy->find_max(); };

  void modify_key(const char* val, int z) { strategy->modify_key(val, z); };
  void _show() { strategy->_show(); }
  size_t size() const { return strategy->size(); }
};

#endif
