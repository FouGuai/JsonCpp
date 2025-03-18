#ifndef READER_H
#define READER_H

#include "Parser.h"
#include "Value.h"
#include <string>
// #include "config.h"

class JSON Reader {
private:

    String text;
    
public:
    Reader(const String &text) : text(text) {};
    Reader() = default;
    Value read_from_text(const String &text);
    Value read_from_file(const String &path);
    Value read();
};



#endif // READER_H