#ifndef TESTY_HH
#define TESTY_HH
#include <random>
#include <chrono>

#define SREDNIA 15

void generujDane(Pair *tablica, size_t rozmiar, int seed, int minASCII, int maxASCII){
  std::mt19937 generator(seed);
  std::uniform_int_distribution<int> klucze(0, 1000000);
  std::uniform_int_distribution<int> wartosci(minASCII, maxASCII);
  for(size_t i=0;i<rozmiar;i++){
    int klucz = klucze(generator);
    char wartosc[6];
    for(int j=0;j<5;j++){
      wartosc[j] = static_cast<char>(wartosci(generator));
    }
    wartosc[5] = '\0';
    tablica[i] = Pair(klucz, wartosc);
  }
}

//ogolnie kwestia tego jakie sie da dane, zwraca tablice intow ze zmierzonymi czasami (dla kazdego, co 500???)

//zapisuje dane do podanej tablicy czasy (zeby nie zwracac wskaznika bo zaalokowany tu, zwalniany tam -- syf.
//nie da sie dodać, usunac i dodac jeszcze raz (zeby dzialac na tej samej z całą pewnościa) -- tak wiec zeby policzyc
//srednia dodaje WSZYSTKO i mierze kazda operacje, usuwam WSZYSTKO, potem znowu dodaje WSZYSTKO...
void test_insert(std::unique_ptr<PriorityQueueStrategy> st, Pair* dane, double* czasy, const size_t roz) {
  Prique kolejka(st);
  for(size_t s = 0; s < SREDNIA; ++s) { //zdaje sobie sprawe ze uzycie s, nie j ani k to troche cursed ale tak jak (s)rednia : c
    for(size_t i = 0; i < roz; ++i) {
      auto start = std::chrono::high_resolution_clock::now();
      kolejka.insert(dane[i]); 
      auto end = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double, std::milli> czas = end - start;
      czasy[i] += czas.count();
    }
    for(size_t i = 0; i < roz; ++i) { //posprzatanie całej
      kolejka.extract_max();
    }
  }
  for(size_t i = 0; i < roz; ++i) { //tak wiem ze to trzecia petla i niezbyt wydajne, ale w imie nauki, czy tam 5.5 (mozna uzyc sredniej kroczacej, ale to syf i robi bledy : p
    czasy[i] /= SREDNIA;
  }
}

//no to samo ale w druga strone, de facto na upartego mozna oba zrobic w jednym bo wystarczy mierzyc tamto extract co i tak
//wywoluje AAAAAALE komu by sie chcial tyle robic -- niech komputer sie pomeczy
void test_extract(std::unique_ptr<PriorityQueueStrategy> st, Pair* dane, double* czasy, const size_t roz) {
  Prique kolejka(st);
  for(size_t s = 0; s < SREDNIA; ++s) { //zdaje sobie sprawe ze uzycie s, nie j ani k to troche cursed ale tak jak srednia : c
    for(size_t i = 0; i < roz; ++i) {
      kolejka.insert(dane[i]); 
    }
    for(size_t i = 0; i < roz; ++i) {
      auto start = std::chrono::high_resolution_clock::now();
      kolejka.extract_max();
      auto end = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double, std::milli> czas = end - start;
      czasy[i] += czas.count();
    }
  }
  for(size_t i = 0; i < roz; ++i) {
    czasy[i] /= SREDNIA;
  }
}

//analogicznie
void test_find_max(std::unique_ptr<PriorityQueueStrategy> st, Pair* dane, double* czasy, const size_t roz) {
  Prique kolejka(st);
  for(size_t s = 0; s < SREDNIA; ++s) {
    for(size_t i = 0; i < roz; ++i) {
      kolejka.insert(dane[i]); 
    }
    for(size_t i = 0; i < roz; ++i) {
      auto start = std::chrono::high_resolution_clock::now();
      kolejka.find_max();
      auto end = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double, std::milli> czas = end - start;
      czasy[i] += czas.count();
    }
  }
  for(size_t i = 0; i < roz; ++i) {
    czasy[i] /= SREDNIA;
  }
}

//sredni przypadek jak to sredni zawsze jest sredni dla kazdej struktury bez problemu.
//dowolny indeks ktory wystepuje nalezy zmienic na losowa inna wartosc z zakresu
//void test_modify_average(Prique& kolejka, Pair* dane, double* czasy, const size_t roz) {
//}

#endif
