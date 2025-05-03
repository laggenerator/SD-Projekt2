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
  for (size_t i = 0; i < roz; ++i)
    czasy[i] = 0.0;
  
  Prique kolejka(st);
  for(size_t s = 0; s < SREDNIA; ++s) { //zdaje sobie sprawe ze uzycie s, nie j ani k to troche cursed ale tak jak (s)rednia : c
    for(size_t i = 0; i < roz; ++i) {
      auto start = std::chrono::high_resolution_clock::now();
      kolejka.insert(dane[i]); 
      auto end = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double, std::micro> czas = end - start;
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
  for (size_t i = 0; i < roz; ++i)
    czasy[i] = 0.0;
  
  Prique kolejka(st);
  for(size_t s = 0; s < SREDNIA; ++s) { //zdaje sobie sprawe ze uzycie s, nie j ani k to troche cursed ale tak jak srednia : c
    for(size_t i = 0; i < roz; ++i) { //dodajemy wszystkie roz elementow
      kolejka.insert(dane[i]); 
    }
    for(size_t i = 0; i < roz; ++i) {
      auto start = std::chrono::high_resolution_clock::now();
      kolejka.extract_max();
      auto end = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double, std::micro> czas = end - start;
      czasy[roz-(i+1)] += czas.count(); //zapisuje na dobrej pozycji w tabeli (bo pelna kolejke oprozniamy)
    }
  }
  for(size_t i = 0; i < roz; ++i) {
    czasy[i] /= SREDNIA;
  }
}

// Testuje czy optymistyczny kopiec ma sens
void test_wesolego_kopca(std::unique_ptr<PriorityQueueStrategy> st, double* czasy, const size_t roz) {
  for (size_t i = 0; i < roz; ++i)
    czasy[i] = 0.0;
  
  Prique kolejka(st);
  for(size_t s = 0; s < SREDNIA; ++s) { //zdaje sobie sprawe ze uzycie s, nie j ani k to troche cursed ale tak jak srednia : c
    for(size_t i = 0; i < roz; ++i) { //dodajemy wszystkie roz elementow
      kolejka.insert(2137, "testy"); 
    }
    for(size_t i = 0; i < roz; ++i) {
      auto start = std::chrono::high_resolution_clock::now();
      kolejka.extract_max();
      auto end = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double, std::micro> czas = end - start;
      czasy[roz-(i+1)] += czas.count(); //zapisuje na dobrej pozycji w tabeli (bo pelna kolejke oprozniamy)
    }
  }
  for(size_t i = 0; i < roz; ++i) {
    czasy[i] /= SREDNIA;
  }
}

//analogicznie
void test_find_max(std::unique_ptr<PriorityQueueStrategy> st, Pair* dane, double* czasy, const size_t roz) {
  for (size_t i = 0; i < roz; ++i)
    czasy[i] = 0.0;
  
  Prique kolejka(st);
  for(size_t s = 0; s < SREDNIA; ++s) {
    for(size_t i = 0; i < roz; ++i) { //dodajemy, wywoluje find_max, mierzy czas i tak 0 do rozmiar, potem zeruje kolejke
      kolejka.insert(dane[i]);
      
      auto start = std::chrono::high_resolution_clock::now();
      kolejka.find_max();
      auto end = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double, std::micro> czas = end - start;
      czasy[i] += czas.count();
    }
    for(size_t i = 0; i < roz; ++i)
      kolejka.extract_max(); //wyczyszczenie kolejki
  }
  for(size_t i = 0; i < roz; ++i) {
    czasy[i] /= SREDNIA;
  }
}

//potrzebne do testow modify_key
//przypisuje unikalny lancuch znakow do inta w zakresie
const std::string CHARSET = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
const int BASE = CHARSET.size();
const int LENGTH = 5;
void int_to_string(int n, char* wynik) {
    if (n >= 916132832) {
        throw std::out_of_range("za duza liczba!");
    }

    for (int i = LENGTH - 1; i >= 0; --i) {
        wynik[i] = CHARSET[n % BASE];
        n /= BASE;
    }
    wynik[LENGTH] = '\0';
}


//mozna to zrobic madrzej, bez trzech fukncji ale to dziala juz
//sredni przypadek jak to sredni zawsze jest sredni dla kazdej struktury bez problemu.
//dowolny indeks ktory wystepuje nalezy zmienic na losowa inna wartosc z zakresu
//OPTYMISTYCZNY PRZYPADEK: zmiana najwiekszemu na najwiekszego
//PESYMISTYCZNY PRZYPADEK: zmiana najmniejszemu na najwiekszego
//SREDNI PRZYPADEK: zmiana losowemu na losowe
void test_modify_average(std::unique_ptr<PriorityQueueStrategy> st, double* czasy, const size_t roz) {
  for (size_t i = 0; i < roz; ++i)
    czasy[i] = 0.0;
  
  Prique kolejka(st);
  std::mt19937 generator(12345); //bo tak
  for(size_t s = 0; s < SREDNIA; ++s) {
    for(size_t i = 0; i < roz; ++i) {
      char* wartosc = new char[6];
      int_to_string(i, wartosc);
      kolejka.insert(Pair(i, wartosc)); //dodaje z rosnacym o 1 kluczem i unikalnym łańcuchem
      delete[] wartosc;

      //losuje ktoremu zmienic i na jaka wartosc
      std::uniform_int_distribution<int> klucze(0, i);
      char* val = new char[6];
      int zmieniany = klucze(generator);
      int_to_string(zmieniany, val);
      int nowy_klucz = klucze(generator);

      auto start = std::chrono::high_resolution_clock::now();
      kolejka.modify_key(val, nowy_klucz);
      auto end = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double, std::micro> czas = end - start;
      czasy[i] += czas.count();
    }
    for(size_t i = 0; i < roz; ++i) {
      kolejka.extract_max(); //zwalnianie calej
    }
  }
  for(size_t i = 0; i < roz; ++i) {
    czasy[i] /= SREDNIA;
  }
}

//ostatniemu zmieniamy tak zeby byl pierwszy
//ostatni na pierwszy jest gorszy od pierwszy na ostatni, bo trzeba przeszukać całą struture (zakladajac przeszukiwanie
//od poczatku, chociaz mozna to zrobic w kopcu albo tablicy szybciej (wlasnosc kopca, szukanie binarne) w kazdym razie
//chyba to jeszcze bardziej pesymistyczne jest

//mozna tez ostatni na ostatni, bo wtedy duzo przeszukiwania ale z drugiej strony da kopca to jest lepsze niz to wyzej
void test_modify_pessimistic(std::unique_ptr<PriorityQueueStrategy> st, double* czasy, const size_t roz) {
  for (size_t i = 0; i < roz; ++i)
    czasy[i] = 0.0;
  
  Prique kolejka(st);
  std::mt19937 generator(12345); //bo tak
  std::uniform_int_distribution<int> klucze(0, roz-1); //jakby tu dac jakas zaporowa wartosc to by bylo czesto tak ze by dawalo duuuuzy i by go dawalo na koniec lol zawsze
  for(size_t s = 0; s < SREDNIA; ++s) {
    for(size_t i = 0; i < roz; ++i) {
      char* wartosc = new char[6];
      int_to_string(i, wartosc);
      kolejka.insert(Pair(roz-i, wartosc)); //dodaje z malejacym o 1 kluczem (od roz) i unikalnym łańcuchem, na koniec

      //pesymistycznie -- bierzemy najwiekszy i zmieniamy mu na ujemny, za kazdym razem mniejszy (-i)
      //const char* val = kolejka.find_max().get_val();
      
      //pesymistycznie -- bierzemy ostatni najmniejszy (o wartosci int_to_string(i) i kluczu roz-i, robimy zeby byl pierwszy (klucz roz+i)
      auto start = std::chrono::high_resolution_clock::now();
      kolejka.modify_key(wartosc, roz+i);
      auto end = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double, std::micro> czas = end - start;
      czasy[i] += czas.count();
    }
    for(size_t i = 0; i < roz; ++i) {
      kolejka.extract_max(); //zwalnianie calej
    }
  }
  for(size_t i = 0; i < roz; ++i) {
    czasy[i] /= SREDNIA;
  }
}

//najwiekszy na najwiekszy -- najmniej przeszukiwania i najmniej przenoszenia
void test_modify_optimistic(std::unique_ptr<PriorityQueueStrategy> st, double* czasy, const size_t roz) {
  for (size_t i = 0; i < roz; ++i)
    czasy[i] = 0.0;
  
  Prique kolejka(st);
  std::mt19937 generator(12345); //bo tak
  std::uniform_int_distribution<int> klucze(0, roz-1); //jakby tu dac jakas zaporowa wartosc to by bylo czesto tak ze by dawalo duuuuzy i by go dawalo na koniec lol zawsze
  for(size_t s = 0; s < SREDNIA; ++s) {
    for(size_t i = 0; i < roz; ++i) {
      char* wartosc = new char[6];
      int_to_string(i, wartosc);
      kolejka.insert(Pair(i, wartosc)); //dodaje z rosnacym o 1 kluczem i unikalnym łańcuchem
      delete[] wartosc;

      //optymistycznie -- bierzemy najwiekszy i zmieniamy mu na... najwiekszy, jeszcze wiekszy niz mial (bo ma roz-1)
      const char* val = kolejka.find_max().get_val();

      auto start = std::chrono::high_resolution_clock::now();
      kolejka.modify_key(val, roz+i);
      auto end = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double, std::micro> czas = end - start;
      czasy[i] += czas.count();
    }
    for(size_t i = 0; i < roz; ++i) {
      kolejka.extract_max(); //zwalnianie calej
    }
  }
  for(size_t i = 0; i < roz; ++i) {
    czasy[i] /= SREDNIA;
  }
}

void test_size(std::unique_ptr<PriorityQueueStrategy> st, Pair* dane, double* czasy, const size_t roz) {
  for (size_t i = 0; i < roz; ++i)
    czasy[i] = 0.0;
  
  Prique kolejka(st);
  for(size_t s = 0; s < SREDNIA; ++s) {
    for(size_t i = 0; i < roz; ++i) { //dodajemy, wywoluje size, mierzy czas i tak 0 do rozmiar, potem zeruje kolejke
      kolejka.insert(dane[i]);
      
      auto start = std::chrono::high_resolution_clock::now();
      kolejka.size();
      auto end = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double, std::micro> czas = end - start;
      czasy[i] += czas.count();
    }
    for(size_t i = 0; i < roz; ++i)
      kolejka.extract_max(); //wyczyszczenie kolejki
  }
  for(size_t i = 0; i < roz; ++i) {
    czasy[i] /= SREDNIA;
  }
}



#endif