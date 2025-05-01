#include <iostream>
#include <algorithm>
#include <fstream>
#include "prique.hh"
#include "testy.hh"

#define N_TESTU 1000 //od 0 do 999 elementow : ) - potem sie zrobi to do 50 kafli imo znowu

void zapisz(const char* nazwa_pliku, double dane[3][N_TESTU]) {
  //zapis do pliku
  std::ofstream plik;
  plik.open(nazwa_pliku, std::ios::out | std::ios::trunc);
  if (!plik.is_open()) {
    std::cerr << "Nie można otworzyć pliku: " << nazwa_pliku << std::endl;
  }
  plik << "rozmiar;kopiec;lista;tablica" << std::endl;
  
  //ogolnie pierwsza kolumna to po prostu i, ale np w usuwaniu to musi byc i+1, w insert juz i
  for(size_t i = 0; i < N_TESTU; ++i) {
    plik << i;
    for(size_t j = 0; j < 3; ++j) {
      plik << ";" << dane[j][i];
    }
    plik << std::endl;
  }
  plik.close();
}

std::unique_ptr<PriorityQueueStrategy> strategia(int i) {
  switch(i) {
  case 0:
    return std::make_unique<HeapStrategy>();
    break;
  case 1:
    return std::make_unique<ListStrategy>();
    break;
  case 2:
    return std::make_unique<DescendArrayStrategy>();
  }
  return nullptr; // lepiej wyjatek aleee juz tak niech bedzie
}

int main() {
  //testy insert
  int ziarno = 2010370;
  Pair dane[N_TESTU];
  double insertAVG[3][N_TESTU]; //heap, list, array
  double insertOPT[3][N_TESTU]; //heap, list, array
  double insertPES[3][N_TESTU]; //heap, list, array


  double extractAVG[3][N_TESTU]; //heap, list, array
  double extractOPT[3][N_TESTU]; //heap, list, array
  double extractPES[3][N_TESTU]; //heap, list, array


  //testy insert
  // pesymistyczny dla listy, tablicy to dodawanie ciagle najmniejszego -- daje na koniec sam (dane posortowane od najw. do najm.)
  // pesymistyczny dla kopca to dodawania ciagle najwiekszego -- musi caaaaaaly kopiec przekopac (dane posortowane od najm. do najw.)
  // optymistyczne z kolei na odwrot
  for(int i = 0; i < 3; ++i) {
    generujDane(dane, N_TESTU, ziarno, 'a', 'z');
    test_insert(strategia(i), dane, insertAVG[i], N_TESTU);
    
    std::sort(dane, dane+N_TESTU, [](Pair a, Pair b) { return a > b; }); //od najw do najm
    test_insert(strategia(i), dane, (i == 0 ? insertOPT[i] : insertPES[i]), N_TESTU);
    
    std::sort(dane, dane+N_TESTU, [](Pair a, Pair b) { return a < b; }); //od najm do najw
    test_insert(strategia(i), dane, (i == 0 ? insertPES[i] : insertOPT[i]), N_TESTU);
  }

  zapisz("pomiary/insert_srednie.csv", insertAVG);
  zapisz("pomiary/insert_optymistyczne.csv", insertOPT);
  zapisz("pomiary/insert_pesymistyczne.csv", insertPES);


  //testy sciagania (extract_max)
  // pesymistycznie
  // pesymistycznie
  // pesymistycznie
  for(int i = 0; i < 3; ++i) {
    generujDane(dane, N_TESTU, ziarno, 'a', 'z');
    test_insert(strategia(i), dane, extractAVG[i], N_TESTU);
    
    std::sort(dane, dane+N_TESTU, [](Pair a, Pair b) { return a > b; }); //od najw do najm
    test_insert(strategia(i), dane, (i == 0 ? extractOPT[i] : extractPES[i]), N_TESTU);
    
    std::sort(dane, dane+N_TESTU, [](Pair a, Pair b) { return a < b; }); //od najm do najw
    test_insert(strategia(i), dane, (i == 0 ? extractPES[i] : extractOPT[i]), N_TESTU);
  }

  zapisz("pomiary/extract_srednie.csv", extractAVG);
  zapisz("pomiary/extract_optymistyczne.csv", extractOPT);
  zapisz("pomiary/extract_pesymistyczne.csv", extractPES);
  return 0;
}
