#include "generator.hh"
void generujDane(Pair *tablica, size_t rozmiar, int seed, int minASCII, int maxASCII){
  std::mt19937 generator(seed);
  std::uniform_int_distribution<int> klucze(0, 1000000); // Klucze są od 0 do miliona
  std::uniform_int_distribution<char> wartosci(minASCII, maxASCII); // Znaki od A do Z

  for(size_t i=0;i<rozmiar;i++){
    int klucz = klucze(generator);
    char wartosc[6]; // 5 znaków i "\0"
    for(int j=0;j<5;j++){
      wartosc[j] = wartosci(generator);
    }
    wartosc[5] = '\0';

    tablica[i] = Pair(klucz, wartosc);
  }
}