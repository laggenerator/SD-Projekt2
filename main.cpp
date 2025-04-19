#include <iostream>
#include "heap.hh"
#include "list.hh"
#include "dynamic_array.hh"
#include "prique.hh"
#include "generator.hh"
#include "testy.hh"


int main(){
  Prique lista(std::make_unique<ListStrategy>());
  for(size_t i=0;i<10;i++){
    lista.insert(i, "test");
  }
  for(size_t i=10;i>0;i++){
    lista.extract_max();
  }
  return 0;
  // ROZPIERDALA SIE COS ZOBACZ LISTE inne dzialaja
}

// int main(int argc, char* argv[]){
//   if(argc != 2){
//     std::cout << "Wszystkie testy [0] |\nPush_front() [1]| Push_back() [2] | Push_at() [3] |\nRemove_front() [4] | Remove_back() [5] | Remove_at() [6] |\nat_position() [7] | find() [8]\n";
//     return 1;
//   }
//   int test = std::stoi(argv[1]);
//   if(test < 0 || test > 5){
//     std::cout << "Niewłaściwa wartość argumentu\n";
//     return 1;
//   }
//   int (*funkcje[])() = {
//     insert, extract_max/*, size, peek, modify_key*/
//   };

  
//   if(test == 0){
//     for(int i=0;i<2;i++){
//       funkcje[i]();
//       std::cout << i+1 << "/" << 5 << std::endl;
//     }
//     std::cout << "Sukces!\n";
//   } else {
//     funkcje[test-1]();
//     std::cout << "Sukces!\n";
//   }
//   return 0;
// }
