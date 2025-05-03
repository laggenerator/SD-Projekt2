#include <iostream>
#include <algorithm>
#include <fstream>
#include <cmath>
#include "prique.hh"
#include "testy.hh"

#define N_TESTU 10000 //od 0 do 9999 elementow
#define N_ZAPIS 25 //co ile zapisywac do pliku

void zapisz(const char* nazwa_pliku, double dane[3][N_TESTU]) {
  //zapis do pliku
  std::ofstream plik;
  plik.open(nazwa_pliku, std::ios::out | std::ios::trunc);
  if (!plik.is_open()) {
    std::cerr << "Nie można otworzyć pliku: " << nazwa_pliku << std::endl;
  }
  plik << "rozmiar;kopiec;lista;tablica" << std::endl;
  //ogolnie pierwsza kolumna to po prostu i, ale np w usuwaniu to musi byc i+1, w insert juz i
  for(size_t i = 1; i < N_TESTU; i+=N_ZAPIS) {
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
    return std::make_unique<HeapStrategy>(); //kopcowo
    break;
  case 1:
    return std::make_unique<ListStrategy>(); //od najwiekszego do najmniejszego
    break;
  case 2:
    return std::make_unique<DescendArrayStrategy>(); //od najwiekszego do najmniejszego
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
  // pesymistyczny dla listy to dodawanie ciagle najmniejszego -- daje na koniec sam (dane posortowane od najw. do najm.)
  // dla tablicy pesymistyczny to jak musi dodać na początek, bo przenosi wszystko (czyli dane posortowane od najm. do najw) 
  // pesymistyczny dla kopca to dodawania ciagle najwiekszego -- musi caaaaaaly kopiec przekopac (dane posortowane od najm. do najw.)
  // optymistyczne z kolei na odwrot
  for(int i = 0; i < 3; ++i) {
    generujDane(dane, N_TESTU, ziarno, 'a', 'z');
    test_insert(strategia(i), dane, AVG[i], N_TESTU);
    std::sort(dane, dane+N_TESTU, [](Pair a, Pair b) { return a > b; }); //od najw do najm
    test_insert(strategia(i), dane, (i == 1 ? PES[i] : OPT[i]), N_TESTU);
    std::sort(dane, dane+N_TESTU, [](Pair a, Pair b) { return a < b; }); //od najm do najw
    test_insert(strategia(i), dane, (i == 1 ? OPT[i] : PES[i]), N_TESTU);
  }

  zapisz("pomiary/insert_srednie.csv", AVG);
  zapisz("pomiary/insert_optymistyczne.csv", OPT);
  zapisz("pomiary/insert_pesymistyczne.csv", PES);
  std::cout << "koniec insert" << std::endl;

  //testy sciagania (extract_max)
  // pesymistycznie ani optymistycznie dla listy i tablicy sie nie da, bo i tak zawsze element sciagany jest pierwszy O(1)
  // pesymistycznie dla kopca oznacza tyle, że jak juz ostatni da na pierwszy i usunie ostatni to wtedy musi ten ostatni na sam
  // koniec przepchac, optymistycznie to musi jak najmniej przepchac

  //ogolnie nie wiem czy zrobimy bo ciezka sprawa i chyba bez sensu nawet to robic lol
  // Nie ma jak bo zawsze bedzie cos do naprawienia i wyjdzie logn
  for(int i = 0; i < 3; ++i) {
    generujDane(dane, N_TESTU, ziarno, 'a', 'z');
    test_extract(strategia(i), dane, AVG[i], N_TESTU);
    if(i==0){
      test_wesolego_kopca(strategia(i), OPT[i], N_TESTU);
    } 
    // else {
    //   for(int j=0;i<N_TESTU;i++){
    //     OPT[i][j] = 0;
    //   }
    // }

  }

  zapisz("pomiary/extract_srednie.csv", AVG);
  zapisz("pomiary/extract_wesoly_kopiec.csv", OPT);
  std::cout << "koniec extract" << std::endl;

  //testy podgladania :~ D, find_max
  // wszystko w czasie O(1), zawsze
  for(int i = 0; i < 3; ++i) {
    generujDane(dane, N_TESTU, ziarno, 'a', 'z');
    test_find_max(strategia(i), dane, AVG[i], N_TESTU);
  }

  zapisz("pomiary/find_max_srednie.csv", AVG);
  std::cout << "koniec finda" << std::endl;

  //testy modyfikacji, modify_key
  // optymistycznie to jak za duzo nie trzeba zmieniac -- tutaj po prostu zwiekszenie klucza juz i tak najwiekszemu
  // pesymistcznie to jak duzo trzeba zmieniac -- tutaj zmiana największemu (find_max) na nowy najmniejszy
  // srednio to obojetnie ktoremu na costam sie zmienia
  for(int i = 0; i < 3; ++i) {
    generujDane(dane, N_TESTU, ziarno, 'a', 'z');
    test_modify_average(strategia(i), AVG[i], N_TESTU);
    test_modify_pessimistic(strategia(i), PES[i], N_TESTU);
    test_modify_optimistic(strategia(i), OPT[i], N_TESTU);
  }

  zapisz("pomiary/modify_srednie.csv", AVG);
  zapisz("pomiary/modify_pesymistyczne.csv", PES);
  zapisz("pomiary/modify_optymistyczne.csv", OPT);
  std::cout << "koniec modify" << std::endl;

  //testy size
  // jeden przypadek, bo to jest przetrzymywane w zmiennej
  for(int i = 0; i < 3; ++i) {
    generujDane(dane, N_TESTU, ziarno, 'a', 'z');
    test_size(strategia(i), dane, AVG[i], N_TESTU);
  }

  zapisz("pomiary/size.csv", AVG);
  std::cout << "koniec size" << std::endl;

  return 0;
}
