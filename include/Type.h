#ifndef TYPE_H
#define TYPE_H
#include "config.h"
#include <string>
#include <vector>
#include <map>

class Value;
using String = std::string;
using Key = String;
using Object = std::map<Key, Value>;
using Array = std::vector<Value>;
using Number = long long;

#endif