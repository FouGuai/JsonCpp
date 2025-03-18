#include "ValueAlgorithm.h"

std::vector<Value::iterator> find(Value::iterator begin, Value::iterator end, const Key &key)
{
    std::vector<Value::iterator> rls;
    for (auto it = begin; it != end; ++it)
    {
        if (it->have_key() && it->get_key() == key)
        {
            rls.push_back(it);
        }
    }

    return rls;
}