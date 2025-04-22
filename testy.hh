#ifndef TESTY_H
#define TESTY_H

#include <fstream>
#include <iostream>
#include <chrono>
#include <stdint.h>
#include <sstream>

enum trybTestu {
  optymistyczny,
  sredni,
  pesymistyczny
};

int insert(trybTestu scenariusz);
int extract_max(trybTestu scenariusz);
int size(trybTestu scenariusz);
int peek(trybTestu scenariusz);
int modify_key(trybTestu scenariusz);

#endif