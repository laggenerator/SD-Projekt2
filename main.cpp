#include <iostream>
#include "heap.hh"
#include "list.hh"
#include "dynamic_array.hh"
#include "prique.hh"
#include "generator.hh"
#include "testy.hh"


int main(int argc, char* argv[]){
  if(argc != 3){
    std::cout << "Wszystkie testy [0] |\nInsert() [1]| Extract_max() [2] | Peek() [3] |\nSize() [4] | Modify_key() [5]\n";
    std::cout << "Optymistyczny [0] | Sredni [1] | Pesymistyczny [2]\n";
    return 1;
  }
  int test = std::stoi(argv[1]);
  int scen = std::stoi(argv[2]);
  if(test < 0 || test > 5){
    std::cerr << "Niewłaściwa wartość testu\n";
    return 1;
  }
  if(scen < 0 || scen > 2){
    std::cerr << "Niewłaściwa wartość scenariusza";
    return 1;
  }
  trybTestu scenariusz;

  switch (scen)
  {
  case 0:
    scenariusz = optymistyczny;
    break;
  case 1:
    scenariusz = sredni;
    break;
  case 2:
    scenariusz = pesymistyczny;
    break;
  default:
    scenariusz = sredni;
    break;
  }
  int (*funkcje[])(trybTestu) = {
    insert, extract_max, peek , size, modify_key
  };

  
  if(test == 0){
    for(int i=0;i<2;i++){
      funkcje[i](scenariusz);
      std::cout << i+1 << "/" << 5 << std::endl;
    }
    std::cout << "Sukces!\n";
  } else {
    funkcje[test-1](scenariusz);
    std::cout << "Sukces!\n";
  }
  return 0;
}
