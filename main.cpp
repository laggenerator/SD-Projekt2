#include <iostream>
#include "heap.hh"
#include "list.hh"
#include "dynamic_array.hh"

int main() {
  Heap kopiec;
  kopiec.build({Pair(1, "cos"), Pair(4, "cos"), Pair(10, "cos"), Pair(2, "aha"), Pair(0, "krowa"), Pair(12, "kot"), Pair(8, "pies"), Pair(7, "tan"), Pair(3, "sin")});
  
  kopiec._show();

  kopiec.decrease_key("krowa", 10);
  
  while(kopiec.size() > 0) {
    std::cout << kopiec.extract_max() << std::endl;
  }
  return 0;
}
