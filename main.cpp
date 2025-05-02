#include <iostream>
#include <algorithm>
#include <fstream>
#include "prique.hh"
#include "testy.hh"

#define N_TESTU 50000 //od 0 do 999 elementow : )

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
  double AVG[3][N_TESTU]; //heap, list, array
  double OPT[3][N_TESTU]; //heap, list, array
  double PES[3][N_TESTU]; //heap, list, array

  //testy insert
  // pesymistyczny dla listy, tablicy to dodawanie ciagle najmniejszego -- daje na koniec sam (dane posortowane od najw. do najm.)
  // pesymistyczny dla kopca to dodawania ciagle najwiekszego -- musi caaaaaaly kopiec przekopac (dane posortowane od najm. do najw.)
  // optymistyczne z kolei na odwrot
  for(int i = 0; i < 3; ++i) {
    generujDane(dane, N_TESTU, ziarno, 'a', 'z');
    test_insert(strategia(i), dane, AVG[i], N_TESTU);
    
    std::sort(dane, dane+N_TESTU, [](Pair a, Pair b) { return a > b; }); //od najw do najm
    test_insert(strategia(i), dane, (i == 0 ? OPT[i] : PES[i]), N_TESTU);
    
    std::sort(dane, dane+N_TESTU, [](Pair a, Pair b) { return a < b; }); //od najm do najw
    test_insert(strategia(i), dane, (i == 0 ? PES[i] : OPT[i]), N_TESTU);
  }

  zapisz("pomiary/insert_srednie.csv", AVG);
  zapisz("pomiary/insert_optymistyczne.csv", OPT);
  zapisz("pomiary/insert_pesymistyczne.csv", PES);


  //testy sciagania (extract_max)
  // pesymistycznie ani optymistycznie dla listy i tablicy sie nie da, bo i tak zawsze element sciagany jest pierwszy O(1)
  // pesymistycznie dla kopca oznacza tyle, że jak juz ostatni da na pierwszy i usunie ostatni to wtedy musi ten ostatni na sam
  // koniec przepchac, optymistycznie to musi jak najmniej przepchac

  //ogolnie nie wiem czy zrobimy bo ciezka sprawa i chyba bez sensu nawet to robic lol
  for(int i = 0; i < 3; ++i) {
    generujDane(dane, N_TESTU, ziarno, 'a', 'z');
    test_extract(strategia(i), dane, AVG[i], N_TESTU);
  }

  zapisz("pomiary/extract_srednie.csv", AVG);

  //testy podgladania :~ D, find_max
  // wszystko w czasie O(1), zawsze
  for(int i = 0; i < 3; ++i) {
    generujDane(dane, N_TESTU, ziarno, 'a', 'z');
    test_find_max(strategia(i), dane, AVG[i], N_TESTU);
  }

  zapisz("pomiary/find_max_srednie.csv", AVG);

   //testy modyfikacji, modify_key
  // 
  for(int i = 0; i < 3; ++i) {
    generujDane(dane, N_TESTU, ziarno, 'a', 'z');
    test_find_max(strategia(i), dane, AVG[i], N_TESTU);
  }

  zapisz("pomiary/find_max_srednie.csv", AVG);
  return 0;
}
