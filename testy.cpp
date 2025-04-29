#include "testy.hh"

#include "generator.hh"
#include "prique.hh"
#include "pair.hh"
#include "algorithm"

const size_t rozmiar = 50000;
int min = 60;
int max = 122;
const size_t ilesrednia = 1; //z ilu pomiarow liczymy srednia
const size_t coilelog = 100; //co ile bierzemy probke, dla 50 000 da to 500 próbek
uint16_t nasiona[5] = {2137, 1410, 420, 1918, 966};

auto start = std::chrono::high_resolution_clock::now();
auto stop = std::chrono::high_resolution_clock::now();
auto czas_lista = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
auto czas_tablicamalejaca = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
auto czas_tablicarosnaca = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
auto czas_kopiec = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);

void przygotujDane(Pair* dane, size_t rozmiar, uint16_t seed, trybTestu scenariusz){
  generujDane(dane, rozmiar, seed, 'A', 'Z');
  if(scenariusz == optymistyczny){ // Największy pierwszy
    std::sort(dane, dane + rozmiar, [](Pair& a, Pair& b){
      return a.get_key() >= b.get_key();
    });
  } else if(scenariusz == pesymistyczny){ // Najmniejszy pierwszy
    std::sort(dane, dane + rozmiar, [](Pair& a, Pair& b){
      return a.get_key() < b.get_key();
    });
  }
  // Średni przypadek zostaje losowy
}

int insert(trybTestu scenariusz){
  std::ostringstream filename;
  filename << "insert_" << scenariusz << ".csv";
  std::ofstream output(filename.str());
  if (!output.is_open()) {
    std::cerr << "Błąd otwierania." << std::endl;
    return 1;
  }
  output << "n;Lista;Tablica malejąca;Kopiec\n";
  Pair daneStartowe[rozmiar];
  std::chrono::nanoseconds pomiarylista[rozmiar/coilelog] {};
  std::chrono::nanoseconds pomiarytablicamalejaca[rozmiar/coilelog] {};
  std::chrono::nanoseconds pomiarykopiec[rozmiar/coilelog] {};
  for(size_t j=0;j<ilesrednia;j++){
    przygotujDane(daneStartowe, rozmiar, nasiona[j], scenariusz);
    std::cout << daneStartowe[0] << ";" << daneStartowe[1] << ";" << daneStartowe[2] <<"\n";
    Prique lista(std::make_unique<ListStrategy>());
    Prique tablicamalejaca(std::make_unique<DescendArrayStrategy>());
    Prique kopiec(std::make_unique<HeapStrategy>());

    for(size_t i=0;i<rozmiar;i++){
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
        kopiec.insert(daneStartowe[i]);
        stop = std::chrono::high_resolution_clock::now();
        pomiarykopiec[i/coilelog] += std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
        
      } else {
        lista.insert(daneStartowe[i]);
        tablicamalejaca.insert(daneStartowe[i]);
        kopiec.insert(daneStartowe[i]);
      }
    }
  }
  // std::cout << pomiarylista[0].count() << " " << pomiarylista[1000].count() << "\n";
  for(size_t i=0;i<rozmiar/coilelog;i++){
    output << i*coilelog << ";" 
           << pomiarylista[i].count()/ilesrednia << ";" 
           << pomiarytablicamalejaca[i].count()/ilesrednia << ";" 
           << pomiarykopiec[i].count()/ilesrednia << "\n";
  }
  output.close();
  return 0;
}

int extract_max(trybTestu scenariusz){
  std::ostringstream filename;
  filename << "extract_" << scenariusz << ".csv";
  std::ofstream output(filename.str());
  if (!output.is_open()) {
    std::cerr << "Błąd otwierania." << std::endl;
    return 1;
  }
  output << "n;Lista;Tablica malejąca;Kopiec\n";
  Pair daneStartowe[rozmiar];
  std::chrono::nanoseconds pomiarylista[rozmiar/coilelog] {};
  std::chrono::nanoseconds pomiarytablicamalejaca[rozmiar/coilelog] {};
  std::chrono::nanoseconds pomiarykopiec[rozmiar/coilelog] {};
  
  for(size_t j=0;j<ilesrednia;j++){
    // przygotujDane(daneStartowe, rozmiar, nasiona[j], scenariusz);
    generujDane(daneStartowe, rozmiar, nasiona[j], 'A', 'Z');

    Prique lista(std::make_unique<ListStrategy>());
    Prique tablicamalejaca(std::make_unique<DescendArrayStrategy>());
    Prique kopiec(std::make_unique<HeapStrategy>());
    for(size_t i=0;i<rozmiar;i++){
      lista.insert(daneStartowe[i]);
      tablicamalejaca.insert(daneStartowe[i]);
      kopiec.insert(daneStartowe[i]);
    }
    for(size_t i=rozmiar;i>0;i--){
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
        kopiec.extract_max();
        stop = std::chrono::high_resolution_clock::now();
        pomiarykopiec[i/coilelog] += std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
        
      } else {
        lista.extract_max();
        tablicamalejaca.extract_max();
        kopiec.extract_max();
      }
    }
  }
  for(size_t i=0;i<rozmiar/coilelog;i++){
    output << rozmiar - i*coilelog << ";" 
          << pomiarylista[i].count()/ilesrednia << ";" 
          << pomiarytablicamalejaca[i].count()/ilesrednia << ";" 
          << pomiarykopiec[i].count()/ilesrednia << "\n";
  }
  output.close();
  return 0;
}
int peek(trybTestu scenariusz){
  std::ostringstream filename;
  filename << "peek_" << scenariusz << ".csv";
  std::ofstream output(filename.str());
  if (!output.is_open()) {
      std::cerr << "Błąd otwierania pliku." << std::endl;
      return 1;
  }
  output << "n;Lista;Tablica malejąca;Kopiec\n";

  Pair daneStartowe[rozmiar];
  std::chrono::nanoseconds pomiarylista[rozmiar/coilelog] = {};
  std::chrono::nanoseconds pomiarytabmalejaca[rozmiar/coilelog] = {};
  std::chrono::nanoseconds pomiarykopiec[rozmiar/coilelog] = {};
  for(size_t j=0;j<ilesrednia;j++){
    // przygotujDane(daneStartowe, rozmiar, nasiona[j], scenariusz);
    generujDane(daneStartowe, rozmiar, nasiona[j], 'A', 'Z');

    Prique lista(std::make_unique<ListStrategy>());
    Prique tablicamalejaca(std::make_unique<DescendArrayStrategy>());
    Prique kopiec(std::make_unique<HeapStrategy>());
    for(size_t i=0;i<rozmiar;i++){
      if(i%coilelog == 0){
        lista.insert(daneStartowe[i]);
        start = std::chrono::high_resolution_clock::now();
        lista.find_max();
        stop = std::chrono::high_resolution_clock::now();
        pomiarylista[i/coilelog] += std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
        
        tablicamalejaca.insert(daneStartowe[i]);
        start = std::chrono::high_resolution_clock::now();
        tablicamalejaca.find_max();
        stop = std::chrono::high_resolution_clock::now();
        pomiarytabmalejaca[i/coilelog] += std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
        
        kopiec.insert(daneStartowe[i]);
        start = std::chrono::high_resolution_clock::now();
        kopiec.find_max();
        stop = std::chrono::high_resolution_clock::now();
        pomiarykopiec[i/coilelog] += std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
        
      } else {
        lista.insert(daneStartowe[i]);
        tablicamalejaca.insert(daneStartowe[i]);
        kopiec.insert(daneStartowe[i]);
      }
    }
  }
  for(size_t i=0;i<rozmiar/coilelog;i++){
    output << i*coilelog << ";" 
          << pomiarylista[i].count()/ilesrednia << ";" 
          << pomiarytabmalejaca[i].count()/ilesrednia << ";" 
          << pomiarykopiec[i].count()/ilesrednia << "\n";
  }
  output.close();
  return 0;
}

int size(trybTestu scenariusz){
  std::ostringstream filename;
  filename << "size_" << scenariusz << ".csv";
  std::ofstream output(filename.str());
  if (!output.is_open()) {
      std::cerr << "Błąd otwierania pliku." << std::endl;
      return 1;
  }
  output << "n;Lista;Tablica malejąca;Kopiec\n";

  Pair daneStartowe[rozmiar];
  std::chrono::nanoseconds pomiarylista[rozmiar/coilelog] = {};
  std::chrono::nanoseconds pomiarytabmalejaca[rozmiar/coilelog] = {};
  std::chrono::nanoseconds pomiarykopiec[rozmiar/coilelog] = {};
  for(size_t j=0;j<ilesrednia;j++){
    przygotujDane(daneStartowe, rozmiar, nasiona[j], scenariusz);

    Prique lista(std::make_unique<ListStrategy>());
    Prique tablicamalejaca(std::make_unique<DescendArrayStrategy>());
    Prique kopiec(std::make_unique<HeapStrategy>());
    for(size_t i=0;i<rozmiar;i++){
      if(i%coilelog == 0){
        lista.insert(daneStartowe[i]);
        start = std::chrono::high_resolution_clock::now();
        lista.size();
        stop = std::chrono::high_resolution_clock::now();
        pomiarylista[i/coilelog] += std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
        
        tablicamalejaca.insert(daneStartowe[i]);
        start = std::chrono::high_resolution_clock::now();
        tablicamalejaca.size();
        stop = std::chrono::high_resolution_clock::now();
        pomiarytabmalejaca[i/coilelog] += std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
        
        kopiec.insert(daneStartowe[i]);
        start = std::chrono::high_resolution_clock::now();
        kopiec.size();
        stop = std::chrono::high_resolution_clock::now();
        pomiarykopiec[i/coilelog] += std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
        
      } else {
        lista.insert(daneStartowe[i]);
        tablicamalejaca.insert(daneStartowe[i]);
        kopiec.insert(daneStartowe[i]);
      }
    }
  }
  for(size_t i=0;i<rozmiar/coilelog;i++){
    output << i*coilelog << ";" 
          << pomiarylista[i].count()/ilesrednia << ";" 
          << pomiarytabmalejaca[i].count()/ilesrednia << ";" 
          << pomiarykopiec[i].count()/ilesrednia << "\n";
  }
  output.close();
  return 0;
}
int modify_key(trybTestu scenariusz){
  std::ostringstream filename;
  filename << "size_" << scenariusz << ".csv";
  std::ofstream output(filename.str());
  if (!output.is_open()) {
      std::cerr << "Błąd otwierania pliku." << std::endl;
      return 1;
  }
  output << "n;Lista;Tablica malejąca;Kopiec\n";

  Pair daneStartowe[rozmiar];
  Pair daneModyfikacyjneSredniCase[rozmiar];
  std::chrono::nanoseconds pomiarylista[rozmiar/coilelog] = {};
  std::chrono::nanoseconds pomiarytabmalejaca[rozmiar/coilelog] = {};
  std::chrono::nanoseconds pomiarykopiec[rozmiar/coilelog] = {};
  for(int j=0;j<ilesrednia;j++){
    przygotujDane(daneStartowe, rozmiar, nasiona[j], scenariusz);
    Prique lista(std::make_unique<ListStrategy>());
    Prique tabmalejaca(std::make_unique<DescendArrayStrategy>());
    Prique kopiec(std::make_unique<HeapStrategy>());

    for (size_t i = 0; i < rozmiar; ++i) {
        lista.insert(daneStartowe[i]);
        tabmalejaca.insert(daneStartowe[i]);
        kopiec.insert(daneStartowe[i]);
    }
    int modyfikacje[rozmiar];
    switch(scenariusz) {
        case optymistyczny:
            for (size_t i = 0;i < rozmiar;i++)
                modyfikacje[i] = daneStartowe[i].get_key() + 1; // Minimalna zmiana
            break;
        case pesymistyczny:
            for (size_t i = 0;i < rozmiar;i++)
                modyfikacje[i] = (i < rozmiar/2) ? 1000000 : 0; // Skrajne wartości
            break;
        case sredni:
            generujDane(daneModyfikacyjneSredniCase, rozmiar, nasiona[(j+1)%5], min, max);
            for(size_t i = 0;i<rozmiar;i++){
              modyfikacje[i] = daneModyfikacyjneSredniCase[i].get_key();
            }
            break;
    }
    for(size_t i=0;i<rozmiar;i++){
      const char* val = daneStartowe[i].get_val();
      int nowy_klucz = modyfikacje[i];
      if(i%coilelog == 0){
        
        start = std::chrono::high_resolution_clock::now();
        lista.modify_key(val, nowy_klucz);
        stop = std::chrono::high_resolution_clock::now();
        pomiarylista[i/coilelog] += std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
        
        
        start = std::chrono::high_resolution_clock::now();
        tabmalejaca.modify_key(val, nowy_klucz);
        stop = std::chrono::high_resolution_clock::now();
        pomiarytabmalejaca[i/coilelog] += std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
              
        start = std::chrono::high_resolution_clock::now();
        kopiec.modify_key(val, nowy_klucz);
        stop = std::chrono::high_resolution_clock::now();
        pomiarykopiec[i/coilelog] += std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
        
      } else {
        lista.modify_key(val, nowy_klucz);
        tabmalejaca.modify_key(val, nowy_klucz);
        kopiec.modify_key(val, nowy_klucz);
      }
    }
  }
  for(size_t i=0;i<rozmiar/coilelog;i++){
    output << i*coilelog << ";" 
          << pomiarylista[i].count()/ilesrednia << ";" 
          << pomiarytabmalejaca[i].count()/ilesrednia << ";" 
          << pomiarykopiec[i].count()/ilesrednia << "\n";
  }
  output.close();
  return 0;
}