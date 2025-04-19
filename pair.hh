#ifndef PAIR_H
#define PAIR_H
#include <cstring>
#include <ostream>

#define VAL_SIZE 6

class Pair {
  int key;
  char val[VAL_SIZE];
public:
  Pair(int k, const char* v) : key(k) {
    strncpy(val, v, VAL_SIZE-1);
    val[VAL_SIZE-1] = '\0';
  };
  Pair(const Pair& p) : key(p.key) {
    strncpy(val, p.val, VAL_SIZE-1);
    val[VAL_SIZE-1] = '\0';
  };
  Pair() : key(0) { memset(val, 0, VAL_SIZE); }

  
  int get_key() { return key; }
  const char* get_val() { return val; }
  void set_key(int k) { key = k; }
  void set_val(const char* v) { strncpy(val, v, VAL_SIZE-1); val[VAL_SIZE-1] = '\0'; }

  //operatory dzialające na wartościach klucza
  bool operator==(const Pair& p) const {
    return (key == p.key);
  }
  bool operator!=(const Pair& p) const {
    return (key != p.key);
  }
  bool operator==(const int i) const {
    return (key == i);
  }
  bool operator!=(const int i) const {
    return (key != i);
  }
  bool operator<(const Pair& p) const {
    return (key < p.key);
  }
  bool operator>(const Pair& p) const {
    return (key > p.key);
  }
  bool operator<=(const Pair& p) const {
    return !(*this > p);
  }
  bool operator>=(const Pair& p) const {
    return !(*this < p);
  }
  Pair& operator=(const Pair& p) {
    if (this != &p) {
      key = p.key;
      strncpy(val, p.val, VAL_SIZE-1);
      val[VAL_SIZE-1] = '\0';
    }
    return *this;
  }

  friend std::ostream& operator<<(std::ostream& os, const Pair& p);
};

inline std::ostream& operator<<(std::ostream& os, const Pair& p) {
  os << "(" << p.key << "|" << p.val << ")";
  return os;
}

#endif
