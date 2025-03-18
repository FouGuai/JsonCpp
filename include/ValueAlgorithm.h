#ifndef VALUE_ALGORITHM_H
#define VALUE_ALGORITHM_H

#pragma once

#include "Value.h"

std::vector<Value::iterator> find(Value::iterator begin, Value::iterator end, const Key &key);

template <typename Condition>
std::vector<Value::iterator> find_if(Value::iterator begin, Value::iterator end, Condition con)
{
    std::vector<Value::iterator> rls;
    for (auto it = begin; it != end; ++it)
    {
        if (it->have_key() && con(it->get_key()))
        {
            rls.push_back(it);
        }
    }
    return rls;
}

#endif
