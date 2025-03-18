#include "config.h"

int __indent_step = 0;
int __deci_precision = 0;

void set_indent(int _indent_step) {
    __indent_step = _indent_step;
}

void set_precision(int pre) {
    __deci_precision = pre;
}