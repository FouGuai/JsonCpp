#ifndef CONFIG_H
#define CONFIG_H


#define JSON
// #define NODEBUG
#include "assert.h"

extern int __deci_precision;
extern int __indent_step;
void set_indent(int);
void set_precision(int);


#endif