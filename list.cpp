#include <iostream>
#include "list.hh"

List::List() { head = nullptr; tail = nullptr; size = 0;}
List::~List() {
  Node* iter = head;
  while (iter != nullptr) {
    Node* temp = iter;
    iter = iter->next;
    delete temp;
  }
}

void List::push_back(Pair val) {
  Node* newNode = new Node(val, tail, nullptr);
  if (head == nullptr) { // Lista jest pusta
    head = newNode;
    tail = newNode;
  } else {
    tail->next = newNode;
    tail = newNode;
  }
  size++;
}

void List::push_front(Pair val) {
  Node* newNode = new Node(val, nullptr, head);
  if(head == nullptr) { //dodajemy pierwszy element
    tail = newNode;
  }
  else
    head->prev = newNode;
  head = newNode;
  size++;
}

void List::push_at(const size_t n, Pair val) { //nowy element staje sie n-tym w liscie
  if(n <= size) { //indeksujemy oczywiscie od 0, wiec jeszcze jako n element mozna dodac, po prostu nowy na koncu
    if(n == 0)
      push_front(val);
    else if(n == size)
      push_back(val);
    else {
      Node* iter;
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
      Node* newNode = new Node(val, iter, iter->next);
      iter->next->prev = newNode;
      iter->next = newNode;
      size++;
    }
  }
  else {
    throw std::invalid_argument("Indeks poza zakresem listy!");
  }
}

Pair List::remove_back() {
  if(size == 0)
    throw std::out_of_range("Nie ma czego usuwać!");

  Pair result;
  if(size == 1) {
    result = head->val;
    delete head;
    head = nullptr;
    tail = nullptr;
  }
  else {
    result = tail->val;
    Node* temp = tail;
    tail = tail->prev;
    tail->next = nullptr;
    delete temp;
  }
  size--;
  return result;
}

Pair List::remove_front() {
  if (size == 0) {
    throw std::out_of_range("Nie ma czego usuwać!");
  }

  Pair result = head->val;
  Node* temp = head;
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

Pair List::remove_at(size_t n) {
  if(n >= size)
    throw std::invalid_argument("Indeks poza zakresem listy!");

  Pair result;
  if(n < size) { //indeksujemy oczywiscie od 0, wiec juz n nie mozna usunac: size = 3, 0 -> 1 -> 2 -> NULL
    if(n == 0)
      return remove_front();
    else if(n == size-1)
      return remove_back();
    else {
      Node* iter;
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

Node* List::find(Pair val) const {
  if(size == 0)
    return nullptr;

  Node* iter = head;
  while(iter != nullptr) {
    if(iter->val == val)
      return iter;
    iter = iter->next;
  }
  
  return nullptr;
}

size_t List::find_index(Pair val) const {
  if(size == 0)
    return size;
  
  Node* iter = head;
  int i = 0;
  while(iter->val != val) {
    iter = iter->next;
    ++i;
    if(iter == nullptr)
      return size;
  }
  
  return i;
}

Node* List::at_position(size_t n) const {
  if(n >= size)
    throw std::out_of_range("Indeks jest poza zakresem!");

  Node* iter = head;
  for(size_t i = 0; i < n; ++i)
    iter = iter->next;

  return iter;
}

void List::_show() {
  Node* iter = head;
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
