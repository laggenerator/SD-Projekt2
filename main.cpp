#include <iostream>
#include "heap.hh"
#include "list.hh"
#include "dynamic_array.hh"
#include "heap_prique.hh"

int main() {
  Heap kopiec;
  kopiec.build({Pair(1, "cos"), Pair(4, "cos"), Pair(10, "cos"), Pair(2, "aha"), Pair(0, "krowa"), Pair(12, "kot"), Pair(8, "pies"), Pair(7, "tan"), Pair(3, "sin")});
  
  kopiec._show();

  kopiec.decrease_key("krowa", 10);
  
  while(kopiec.size() > 0) {
    std::cout << kopiec.extract_max() << std::endl;
  }

  std::cout << "Kolejka listowa: " << std::endl;
  Prique kolejka(std::make_unique<ListStrategy>());
  kolejka.insert(1, "cos");
  kolejka.insert(2, "krowa");
  kolejka.insert(5, "pies");
  kolejka.insert(0, "kurwa");
  kolejka.insert(100, "bóbr");

  kolejka.modify_key("kurwa", 200);
  
  for(int i = 0; i < 5; ++i) {
    kolejka._show();
    std::cout << kolejka.extract_max() << std::endl << std::endl;
  }

  std::cout << "Kolejka kopcowa: " << std::endl;
  Prique kolejka2(std::make_unique<HeapStrategy>());
  kolejka2.insert(1, "cos");
  kolejka2.insert(2, "krowa");
  kolejka2.insert(5, "pies");
  kolejka2.insert(0, "kurwa");
  kolejka2.insert(100, "bóbr");

  kolejka2.modify_key("kurwa", 200);
  
  for(int i = 0; i < 5; ++i) {
    kolejka2._show();
    std::cout << kolejka2.extract_max() << std::endl << std::endl;
  }
  
  return 0;
}
