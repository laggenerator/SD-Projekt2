#include "dynamic_array.hh"

DynamicArray::DynamicArray() : dane(nullptr), rozmiar(0), pojemnosc(0) {}

DynamicArray::~DynamicArray() {
  if(dane){
    free(dane);
  }
}

void DynamicArray::shrink(){
  if(rozmiar < pojemnosc / 2 && pojemnosc > 0){
    Pair* nowe_dane = static_cast<Pair*>(realloc(dane, pojemnosc / 2 * sizeof(Pair)));
    if(!nowe_dane){
      throw std::bad_alloc();
    }

    dane = nowe_dane;
    pojemnosc = pojemnosc / 2;
  }
}

void DynamicArray::resize(std::size_t nowa_pojemnosc){
  if(nowa_pojemnosc == 0){
    free(dane);
    dane = nullptr;
    rozmiar = 0;
    pojemnosc = 0;
    return;
  }
    
  Pair* nowe_dane = static_cast<Pair*>(realloc(dane, nowa_pojemnosc * sizeof(Pair)));
  if(!nowe_dane){
    throw std::bad_alloc();
  }

  dane = nowe_dane;
  pojemnosc = nowa_pojemnosc;
  // if(nowa_pojemnosc > rozmiar){ // Ustawia nowe miejsca na 0, zamiast farmazonów
  //     std::memset(dane + rozmiar, 0, (nowa_pojemnosc - rozmiar) * sizeof(int));
  // }
}

void DynamicArray::push_back(Pair wartosc){
  if(rozmiar == pojemnosc){
    resize(pojemnosc == 0 ? 1 : pojemnosc * 2);
  }
  dane[rozmiar++] = wartosc;
}

void DynamicArray::push_front(Pair wartosc){
  if(rozmiar == pojemnosc){
    resize(pojemnosc == 0 ? 1 : pojemnosc * 2);
  }
  for(std::size_t i=rozmiar;i>0;i--){
    dane[i] = dane[i-1];
  }
  dane[0]=wartosc;
  rozmiar++;
}
void DynamicArray::push_at(unsigned int n, Pair wartosc){
  if(n > rozmiar) throw std::out_of_range("n jest poza zakresem tablicy");
  if(rozmiar == pojemnosc){
    resize(pojemnosc == 0 ? 1 : pojemnosc * 2);
  }
  for(std::size_t i=rozmiar;i>n;i--){
    dane[i] = dane[i-1];
  }
  dane[n] = wartosc;
  rozmiar++;
}

Pair DynamicArray::remove_back(){
  if(rozmiar == 0) throw std::out_of_range("Tablica jest pusta");
  Pair wartosc = dane[rozmiar--];
  shrink();
  return wartosc;
}

Pair DynamicArray::remove_front(){
  if(rozmiar == 0) throw std::out_of_range("Tablica jest pusta");
  Pair wartosc = dane[0];
  for(std::size_t i=0;i<rozmiar - 1;i++){
    dane[i] = dane[i+1];
  }
  rozmiar--;
  shrink();
  return wartosc;
}

Pair DynamicArray::remove_at(unsigned int n){
  if(n >= rozmiar) throw std::out_of_range("n jest poza zakresem tablicy");
  Pair wartosc = dane[n];
  for(std::size_t i=n;i<rozmiar - 1;i++){
    dane[i] = dane[i+1];
  }
  rozmiar--;
  shrink();
  return wartosc;
}

int DynamicArray::find(Pair wartosc){
  for(std::size_t i=0;i<rozmiar;i++){
    if(dane[i] == wartosc) return i;
  }
  return -1;
}
Pair DynamicArray::at_position(unsigned int n){
  if(n >= rozmiar) throw std::out_of_range("n jest poza zakresem tablicy");
  return dane[n];
}

void DynamicArray::_show(){
  std::cout << "[";
  for(std::size_t i=0;i<rozmiar;i++){
    std::cout << dane[i];
    if(i != rozmiar - 1) std::cout << "; ";
  }
  std::cout << "]\n";
}

DynamicArray& DynamicArray::operator=(const DynamicArray& other) {
  if (this != &other) { //samoprzypisanie
    //zwolnienie
    delete[] dane;
            
    //alokowanie
    rozmiar = other.rozmiar;
    pojemnosc = other.pojemnosc;
    dane = static_cast<Pair*>(malloc(pojemnosc * sizeof(Pair)));

    //kopiowanie
    for (std::size_t i = 0; i < rozmiar; ++i) {
      dane[i] = other.dane[i];  // Wykorzystuje operator= klasy Pair
    }
  }
  return *this;
}
