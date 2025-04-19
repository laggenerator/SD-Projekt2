#include "testy.hh"

#include "generator.hh"
#include "prique.hh"
#include "pair.hh"

const size_t rozmiar = 50000;
int min = 60;
int max = 122;
const size_t ilesrednia = 10; //z ilu pomiarow liczymy srednia
const size_t coilelog = 100; //co ile bierzemy probke, dla 50 000 da to 500 próbek


auto start = std::chrono::high_resolution_clock::now();
auto stop = std::chrono::high_resolution_clock::now();
auto czas_lista = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
auto czas_tablicamalejaca = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
auto czas_tablicarosnaca = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
auto czas_kopiec = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);

int insert(){
  std::ostringstream filename;
  filename << "insert.csv";
  std::ofstream output(filename.str());
  if (!output.is_open()) {
    std::cerr << "Błąd otwierania." << std::endl;
    return 1;
  }
  output << "n;Lista;Tablica malejąca;Tablica rosnąca;Kopiec\n";
  Pair daneStartowe[rozmiar];
  std::chrono::nanoseconds pomiarylista[rozmiar/coilelog] {};
  std::chrono::nanoseconds pomiarytablicamalejaca[rozmiar/coilelog] {};
  std::chrono::nanoseconds pomiarytablicarosnaca[rozmiar/coilelog] {};
  std::chrono::nanoseconds pomiarykopiec[rozmiar/coilelog] {};
  generujDane(daneStartowe, rozmiar, 2137, 'A', 'Z');
  for(size_t j=0;j<ilesrednia;j++){
    Prique lista(std::make_unique<ListStrategy>());
    Prique tablicamalejaca(std::make_unique<DescendArrayStrategy>());
    Prique tablicarosnaca(std::make_unique<AscendArrayStrategy>());
    Prique kopiec(std::make_unique<HeapStrategy>());
    for(size_t i=0;i<ilesrednia;i++){
      if(i%coilelog == 0){
        start = std::chrono::high_resolution_clock::now();
        lista.insert(daneStartowe[i]);
        stop = std::chrono::high_resolution_clock::now();
        pomiarylista[i/coilelog] += std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
        
        start = std::chrono::high_resolution_clock::now();
        tablicamalejaca.insert(daneStartowe[i]);
        stop = std::chrono::high_resolution_clock::now();
        pomiarytablicamalejaca[i/coilelog] += std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
        
        start = std::chrono::high_resolution_clock::now();
        tablicarosnaca.insert(daneStartowe[i]);
        stop = std::chrono::high_resolution_clock::now();
        pomiarytablicarosnaca[i/coilelog] += std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
        
        start = std::chrono::high_resolution_clock::now();
        kopiec.insert(daneStartowe[i]);
        stop = std::chrono::high_resolution_clock::now();
        pomiarykopiec[i/coilelog] += std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
        
      } else {
        lista.insert(daneStartowe[i]);
        tablicamalejaca.insert(daneStartowe[i]);
        tablicarosnaca.insert(daneStartowe[i]);
        kopiec.insert(daneStartowe[i]);
      }
    }
  }
  for(size_t i=0;i<rozmiar/coilelog;i++){
    output << i*coilelog << ";" << pomiarylista->count()/ilesrednia << ";" << pomiarytablicamalejaca->count()/ilesrednia << ";" << pomiarytablicarosnaca->count()/ilesrednia << ";" << pomiarykopiec->count()/ilesrednia << "\n";
  }
  output.close();
  return 0;
}

int extract_max(){
  std::ostringstream filename;
  filename << "extract.csv";
  std::ofstream output(filename.str());
  if (!output.is_open()) {
    std::cerr << "Błąd otwierania." << std::endl;
    return 1;
  }
  output << "n;Lista;Tablica malejąca;Tablica rosnąca;Kopiec\n";
  Pair daneStartowe[rozmiar];
  std::chrono::nanoseconds pomiarylista[rozmiar/coilelog] {};
  std::chrono::nanoseconds pomiarytablicamalejaca[rozmiar/coilelog] {};
  std::chrono::nanoseconds pomiarytablicarosnaca[rozmiar/coilelog] {};
  std::chrono::nanoseconds pomiarykopiec[rozmiar/coilelog] {};
  generujDane(daneStartowe, rozmiar, 2137, 'A', 'Z');
  for(size_t j=0;j<ilesrednia;j++){
    Prique lista(std::make_unique<ListStrategy>());
    Prique tablicamalejaca(std::make_unique<DescendArrayStrategy>());
    Prique tablicarosnaca(std::make_unique<AscendArrayStrategy>());
    Prique kopiec(std::make_unique<HeapStrategy>());
    for(size_t i=0;i<rozmiar;i++){
      lista.insert(daneStartowe[i]);
      tablicamalejaca.insert(daneStartowe[i]);
      tablicarosnaca.insert(daneStartowe[i]);
      kopiec.insert(daneStartowe[i]);
    }
    for(size_t i=rozmiar;i>1;--i){
      if(i%coilelog == 0){
        start = std::chrono::high_resolution_clock::now();
        lista.extract_max();
        stop = std::chrono::high_resolution_clock::now();
        pomiarylista[i/coilelog] += std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
        
        start = std::chrono::high_resolution_clock::now();
        tablicamalejaca.extract_max();
        stop = std::chrono::high_resolution_clock::now();
        pomiarytablicamalejaca[i/coilelog] += std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
        
        start = std::chrono::high_resolution_clock::now();
        tablicarosnaca.extract_max();
        stop = std::chrono::high_resolution_clock::now();
        pomiarytablicarosnaca[i/coilelog] += std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
        
        start = std::chrono::high_resolution_clock::now();
        kopiec.extract_max();
        stop = std::chrono::high_resolution_clock::now();
        pomiarykopiec[i/coilelog] += std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
        
      } else {
        // lista.extract_max();
        tablicamalejaca.extract_max();
        tablicarosnaca.extract_max();
        kopiec.extract_max();
      }
    }
  }
  for(size_t i=0;i<(rozmiar/coilelog);i++){
    output << i*coilelog << ";" << pomiarylista->count()/ilesrednia << ";" << pomiarytablicamalejaca->count()/ilesrednia << ";" << pomiarytablicarosnaca->count()/ilesrednia << ";" << pomiarykopiec->count()/ilesrednia << "\n";
  }
  output.close();
  return 0;
}
int size();
int peek();
int modify_key();