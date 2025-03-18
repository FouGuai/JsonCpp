#ifndef VALUEITER_H
#define VALUEITER_H

#include "Config.h"
#include "Type.h"
#include <stack>

/**
 * @brief Value 的迭代器， 按照前序遍历的顺序迭代
 *
 */
class JSON ValueIter
{
    friend class Value;
    using ObjectIter = Object::iterator;
    using ArrayIter = Array::iterator;

    friend bool operator==(const ValueIter &lhs, const ValueIter &rhs);
    friend bool operator!=(const ValueIter &lhs, const ValueIter &rhs);

private:
    Value *pos = nullptr;
    Value *root = nullptr;
    bool iter_data = false;
    std::stack<ObjectIter> obj_iter_trace;
    std::stack<ArrayIter> arr_iter_trace;

    bool is_end() const;
    void move_end();
    ValueIter() = default;

public:
    Value &operator*();
    Value *operator->();
    ValueIter &operator++();
    ValueIter operator++(int);
    ValueIter &operator--();
    ValueIter operator--(int);

    void next();
    void last();
};

/**
 * @brief 迭代 Value 中的 Obj, Arr 对象
 * 
 */
class JSON ValueLayerIter
{

    friend class Value;
    friend bool operator==(const ValueLayerIter &lhs, const ValueLayerIter &rhs);
    friend bool operator!=(const ValueLayerIter &lhs, const ValueLayerIter &rhs);

private:
    Value *root;

    Object::iterator obj_iter;
    Array::iterator arr_iter;

    ValueLayerIter() : root(nullptr) {}

public:
    void next();
    void last();
    Value &operator*();
    Value *operator->();
    ValueLayerIter &operator++();
    ValueLayerIter operator++(int);
    ValueLayerIter &operator--();
    ValueLayerIter operator--(int);
};

#endif