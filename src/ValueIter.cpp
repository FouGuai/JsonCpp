#include "ValueIter.h"
#include "Value.h"

void ValueIter::next()
{

    auto judge_and_move_end = [this]() -> bool
    {
        if (pos == root)
        {
            iter_data = true;
            pos = nullptr;
            return true;
        }
        return false;
    };

    if (type(*pos) == Obj || type(*pos) == Arr)
    {
        if (!iter_data)
        {
            // iter_data = true;
            if (type(*pos) == Obj)
            {
                obj_iter_trace.push(pos->get_object().begin());
                pos = &obj_iter_trace.top()->second;
            }
            else
            {
                arr_iter_trace.push(pos->get_array().begin());
                pos = &*arr_iter_trace.top();
            }
            return;
        }

        if (type(*pos) == Obj)
        {
            ObjectIter ori_iter = obj_iter_trace.top();
            obj_iter_trace.pop();
            ori_iter++;
            if (ori_iter == pos->get_object().end())
            {

                // 如果是到了根就直接进入end节点
                if (judge_and_move_end())
                    return;

                pos = pos->get_parent();
                iter_data = true;
                next();
                return;
            }

            obj_iter_trace.push(ori_iter);
            pos = &ori_iter->second;
        }
        else
        {
            ArrayIter ori_iter = arr_iter_trace.top();
            arr_iter_trace.pop();
            ori_iter++;
            if (ori_iter == pos->get_array().end())
            {
                // 如果是到了根就直接进入end节点
                if (judge_and_move_end())
                    return;

                pos = pos->get_parent();
                iter_data = true;
                next();
                return;
            }

            arr_iter_trace.push(ori_iter);
            pos = &*ori_iter;
        }

        iter_data = false;
        return;
    }

    // 如果是到了根就直接进入end节点
    if (judge_and_move_end())
        return;

    pos = pos->get_parent();
    iter_data = true;
    next();
}

void ValueIter::last()
{
    if (is_end())
    {
        move_end();
        return;
    }

    if (type(*pos) == Obj || type(*pos) == Arr)
    {
        if (!iter_data)
        {
            pos = pos->get_parent();
            iter_data = true;
            last();
            return;
        }

        if (type(*pos) == Obj)
        {
            ObjectIter ori_iter = obj_iter_trace.top();
            obj_iter_trace.pop();
            if (ori_iter == pos->get_object().begin())
            {
                iter_data = false;
                return;
            }
            ori_iter--;
            obj_iter_trace.push(ori_iter);
            pos = &ori_iter->second;
        }
        else
        {
            ArrayIter ori_iter = arr_iter_trace.top();
            arr_iter_trace.pop();
            if (ori_iter == pos->get_array().begin())
            {
                iter_data = false;
                return;
            }
            ori_iter--;
            arr_iter_trace.push(ori_iter);
            pos = &*ori_iter;
        }

        if (type(*pos) == Arr)
        {
            arr_iter_trace.push(pos->get_array().end());
            last();
        }
        else if (type(*pos) == Obj)
        {
            obj_iter_trace.push(pos->get_object().end());
            last();
        }

        return;
    }

    pos = pos->get_parent();
    iter_data = true;
    last();
}

Value &ValueIter::operator*()
{
    return *pos;
}

void ValueIter::move_end()
{
    pos = root;
    while (1)
    {
        if (type(*pos) == Obj)
        {
            if (pos->get_object().empty())
                break;
            auto iter = pos->get_object().end();
            --iter;
            pos = &iter->second;
            obj_iter_trace.push(iter);
        }
        else if (type(*pos) == Arr)
        {
            if (pos->get_array().empty())
                break;
            auto iter = pos->get_array().end() - 1;
            pos = &*iter;
            arr_iter_trace.push(iter);
        }
        else
        {
            break;
        }
    }
}

bool ValueIter::is_end() const
{
    return pos == nullptr && iter_data == true;
}

bool operator==(const ValueIter &lhs, const ValueIter &rhs)
{
    return lhs.pos == rhs.pos && lhs.root == rhs.root &&
           lhs.arr_iter_trace == rhs.arr_iter_trace &&
           lhs.obj_iter_trace == rhs.obj_iter_trace;
}

bool operator!=(const ValueIter &lhs, const ValueIter &rhs)
{
    return !(lhs == rhs);
}

ValueIter &ValueIter::operator++()
{
    this->next();
    return *this;
}

ValueIter ValueIter::operator++(int)
{
    ValueIter ori = *this;
    this->next();
    return ori;
}

ValueIter &ValueIter::operator--()
{
    this->last();
    return *this;
}

ValueIter ValueIter::operator--(int)
{
    ValueIter ori = *this;
    this->last();
    return ori;
}

void ValueLayerIter::next()
{
    assert(type(*root) == Obj || type(*root) == Arr);

    if (type(*root) == Obj)
    {
        ++obj_iter;
    }
    else
    {
        ++arr_iter;
    }
}

void ValueLayerIter::last()
{
    assert(type(*root) == Obj || type(*root) == Arr);

    if (type(*root) == Obj)
    {
        --obj_iter;
    }
    else
    {
        --arr_iter;
    }
}

Value &ValueLayerIter::operator*()
{
    assert(type(*root) == Obj || type(*root) == Arr);
    if (type(*root) == Obj)
    {
        return obj_iter->second;
    }
    else
    {
        return *arr_iter;
    }
}
ValueLayerIter &ValueLayerIter::operator++()
{
    next();
    return *this;
}

ValueLayerIter &ValueLayerIter::operator--()
{
    last();
    return *this;
}

ValueLayerIter ValueLayerIter::operator++(int)
{
    ValueLayerIter ori = *this;
    next();
    return ori;
}

ValueLayerIter ValueLayerIter::operator--(int)
{
    ValueLayerIter ori = *this;
    last();
    return ori;
}

bool operator==(const ValueLayerIter &lhs, const ValueLayerIter &rhs)
{
    assert(type(*lhs.root) == Obj || type(*lhs.root) == Arr);
    bool flag = lhs.root == rhs.root;
    if (type(*lhs.root) == Obj)
    {
        flag = flag && lhs.obj_iter == rhs.obj_iter;
    }
    else
    {
        flag = flag && lhs.arr_iter == rhs.arr_iter;
    }

    return flag;
}

bool operator!=(const ValueLayerIter &lhs, const ValueLayerIter &rhs)
{
    return !(lhs == rhs);
}

Value *ValueIter::operator->()
{
    return pos;
}

Value *ValueLayerIter::operator->()
{
    return &operator*();
}