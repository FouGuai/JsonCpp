#include "Value.h"
#include "assert.h"
#include <format>

#pragma region constructor

Value::Value(ValueType type) : _type(type)
{
    switch (type)
    {
    case Obj:
        val.obj = new Object();
        break;
    case Arr:
        val.arr = new Array();
        break;
    case Str:
        val.str = new String();
        break;
    case Num:
        val.num = 0;
        break;
    case Deci:
        val.deci = 0.0;
        break;
    case Bool:
        val.Bool = false;
        break;
    default:
        break;
    }

    key = nullptr;
    parent = nullptr;
}

Value::Value(String *str)
{
    String *new_str;
    new_str = new String(*str);
    val.str = new_str;
    _type = Str;
}

Value::Value(const String &str)
{
    String *new_str = new String(str);
    val.str = new_str;
    _type = Str;
}

Value::Value(Array *arr)
{
    Array *new_arr;
    new_arr = new Array(*arr);

    _type = Arr;
    // 数组元素不能有键值
    for (int i = 0; i < new_arr->size(); ++i)
    {
        (*new_arr)[i].drop_key();
        (*new_arr)[i].parent = this;
    }

    val.arr = new_arr;
}

Value::Value(const Array &arr)
{
    Array *new_arr = new Array(arr);
    _type = Arr;

    // 数组元素不能有键值
    for (int i = 0; i < new_arr->size(); ++i)
    {
        (*new_arr)[i].drop_key();
        (*new_arr)[i].parent = this;
    }

    val.arr = new_arr;
}

Value::Value(Object *obj)
{
    Object *new_obj;
    _type = Obj;
    new_obj = new Object(*obj);

    for (auto it = new_obj->begin(); it != new_obj->end(); ++it)
    {
        it->second.set_key(it->first);
        it->second.parent = this;
    }

    val.obj = new_obj;
}

Value::Value(const Object &obj)
{
    Object *new_obj = new Object(obj);
    _type = Obj;
    for (auto it = new_obj->begin(); it != new_obj->end(); ++it)
    {
        it->second.set_key(it->first);
        it->second.parent = this;
    }

    val.obj = new_obj;
}

Value::Value(Number num)
{
    val.num = num;
    _type = Num;
}

Value::Value(double deci)
{
    val.deci = deci;
    _type = Deci;
}

Value::Value(bool Bol)
{
    val.Bool = Bol;
    _type = Bool;
}

Value::Value(const char *str)
{
    val.str = new String(str);
    _type = Str;
}

//

// 带键值的构造函数
Value::Value(const Key &key, double deci) : Value(deci)
{
    this->key = new Key(key);
}

Value::Value(const Key &key, Number num) : Value(num)
{
    this->key = new Key(key);
}

Value::Value(const Key &key, bool x) : Value(x)
{
    this->key = new Key(key);
}

// 带键值, 使用引用传递的构造函数，全部都为复制
Value::Value(const Key &key, Array *arr) : Value(arr)
{
    this->key = new Key(key);
}

Value::Value(const Key &key, Object *obj) : Value(obj)
{
    this->key = new Key(key);
}

Value::Value(const Key &key, const String &str) : Value(str)
{
    this->key = new Key(key);
}

Value::Value(const Key &key, const Object &obj) : Value(obj)
{
    this->key = new Key(key);
}

Value::Value(const Key &key, const Array &arr) : Value(arr)
{
    this->key = new Key(key);
}

Value::Value(const Key &key, const char *str) : Value(str)
{
    this->key = new Key(key);
}

#pragma endregion

ValueType Value::type() const
{
    return this->_type;
}

Value &Value::operator[](const Key &key)
{
    assert(type() == Obj);
    if (type() != Obj)
    {
        throw ValueException("Value Error: Type isn't key, couldn't get Value using key.");
    }

    Object &obj = get_object();
    if (obj.find(key) == obj.end())
    {
        obj[key] = NULL_VALUE;
        obj[key].set_key(key);
        obj[key].parent = this;
    }

    return obj[key];
}

Value::~Value()
{
    delete_value();
}

#pragma region copy

Value &Value::operator[](int idx)
{
    assert(type() == Arr);
    if (type() != Arr)
    {
        throw ValueException("Value Error: type isn't Arr, couldn't get Value using index.");
    }
    Array &arr = get_array();

    if (idx < 0)
    {
        idx = static_cast<int>(arr.size()) + idx;
    }

    assert(idx >= 0 && idx < static_cast<int>(arr.size()));
    if (idx < 0 || idx > static_cast<int>(arr.size()))
    {
        throw std::out_of_range("Invalid index.");
    }
    return arr[idx];
}

const Value &Value::operator[](int idx) const
{
    assert(type() == Arr);
    if (type() != Arr)
    {
        throw ValueException("Value Error: type isn't Arr, couldn't get Value using index.");
    }
    const Array &arr = get_array();

    if (idx < 0)
    {
        idx = static_cast<int>(arr.size()) + idx;
    }

    assert(idx >= 0 && idx < static_cast<int>(arr.size()));
    if (idx < 0 || idx > static_cast<int>(arr.size()))
    {
        throw std::out_of_range("Invalid index.");
    }
    return arr[idx];
}

Value::Value(const Value &rhs)
{
    _type = rhs.type();

    switch (_type)
    {
    case Obj:
        val.obj = new Object(rhs.get_object());
        {
            Object &obj = get_object();

            // 更新parent
            for (auto it = obj.begin(); it != obj.end(); ++it)
            {
                it->second.parent = this;
            }
        }

        break;
    case Arr:
        val.arr = new Array(rhs.get_array());
        {
            // 更新parent
            for (int i = 0; i < size(); ++i)
            {
                operator[](i).parent = this;
            }
        }

        break;
    case Str:
        val.str = new String(rhs.get_string());
        break;
    case Num:
        val.num = rhs.get_num();
        break;
    case Deci:
        val.deci = rhs.get_deci();
        break;
    case Bool:
        val.Bool = rhs.get_bool();
        break;
    default:
        break;
    }

    if (rhs.have_key())
        set_key(rhs.get_key());

    parent = rhs.get_parent();
    // indent_step = rhs.indent_step;
}

Value::Value(Value &&rhs)
{
    _type = std::move(rhs._type);
    val = std::move(rhs.val);
    key = std::move(rhs.key);
    parent = std::move(rhs.parent);
    fix_parent();
    rhs._type = Null;
}

void Value::fix_parent()
{
    switch (type())
    {
    case Obj:
    {
        Object &obj = get_object();

        // 更新parent
        for (auto it = obj.begin(); it != obj.end(); ++it)
        {
            it->second.parent = this;
        }
    }

    break;
    case Arr:
    {
        // 更新parent
        for (int i = 0; i < size(); ++i)
        {
            operator[](i).parent = this;
        }
    }
    break;
    default:
        break;
    }
}

Value &Value::operator=(Value rhs)
{
    swap(*this, rhs);
    drop_key();
    if (rhs.have_key())
        this->key = new Key(rhs.get_key());
    fix_parent();
    this->parent = rhs.get_parent();
    return *this;
}

#pragma endregion

void Value::delete_value()
{

    switch (type())
    {
    case Obj:
    {
        // Object &obj = get_object();
        delete val.obj;
        val.obj = nullptr;
    }
    break;
    case Arr:
    {
        // Array &arr = get_array();
        delete val.arr;
        val.arr = nullptr;
    }
    break;
    case Str:
    {
        delete val.str;
        val.str = nullptr;
    }
    break;

    case Num:
    case Deci:
    case Bool:
    default:
        break;
    }
    _type = Null;
    drop_key();
}

String get_str_indent(int indent)
{
    String rls;
    while (indent--)
    {
        rls += " ";
    }
    return rls;
}

String encode_string(const String &str)
{
    String rls;
    for (int i = 0; i < str.size(); ++i)
    {
        switch (str[i])
        {
        case '\b':
            rls += "\\b";
            break;
        case '\f':
            rls += "\\f";
            break;
        case '\t':
            rls += "\\t";
            break;
        case '\n':
            rls += "\\n";
            break;
        case '\r':
            rls += "\\r";
            break;
        case '\\':
            rls += "\\\\";
            break;
        case '"':
            rls += "\"";
            break;
        default:
            rls.push_back(str[i]);
            break;
        }
    }
    return rls;
}

String decode_deci(double deci)
{
    return __deci_precision > 0
               ? std::format("{:.{}f}", deci, __deci_precision)
               : std::format("{}", deci);
}

void Value::to_string(String &str, int indent) const
{

    // 添加缩进
    String str_indent = get_str_indent(indent);
    str += str_indent;

    if (have_key())
    {
        str += "\"" + get_key() + "\"" + " : ";
    }

    switch (type())
    {
    case Obj:
    {
        str += "{\n";
        const Object &obj = get_object();
        int i = 0;
        for (auto it = obj.begin(); it != obj.end(); ++it)
        {
            assert(it->second.parent == this);

            // std::cout << it->second.get_key() << ' ' << it->first << '\n';
            assert(it->second.have_key() && it->second.get_key() == it->first);
            if (!(it->second.have_key() && it->second.get_key() == it->first))
            {
                throw ValueException("Value Error: In Object, key and the son's key is different.");
            }

            it->second.to_string(str, indent + __indent_step);
            if (i++ != static_cast<int>(obj.size() - 1))
                str += ",";
            str += "\n";
        }
        str += str_indent;
        str += "}";
    }
    break;
    case Arr:
    {
        str += "[\n";
        auto &arr = get_array();
        for (int i = 0; i < static_cast<int>(arr.size()); ++i)
        {

            assert(arr[i].parent == this);

            arr[i].to_string(str, indent + __indent_step);
            if (i != static_cast<int>(arr.size()) - 1)
            {
                str += ",";
            }
            str += "\n";
        }
        str += str_indent;
        str += "]";
    }
    break;
    case Str:
    {
        str += "\"" + encode_string(get_string()) + "\"";
    }
    break;
    case Num:
    {
        str += std::to_string(get_num());
    }
    break;
    case Deci:
    {
        str += decode_deci(get_deci());
    }
    break;
    case Bool:
    {
        str += get_bool() ? "true" : "false";
    }
    break;
    case Null:
    default:
    {
        str += "null";
    }
    break;
    }
}

void Value::to_string(String *str, int indent) const
{
    this->to_string(*str, indent);
}

String Value::to_string(int indent) const
{
    std::string str;
    to_string(str, indent);
    return str;
}

void Value::set_key(const Key &key)
{
    // 防止内存泄露
    if (have_key())
        delete this->key;
    this->key = new Key(key);
}

void Value::drop_key()
{
    // _have_key = false;

    if (have_key())
        delete this->key;
    this->key = nullptr;
}

size_t Value::size() const
{
    assert(type() == Obj || type() == Arr);
    if (!(type() == Obj || type() == Arr))
    {
        throw ValueException("Value Error: Only Obj and Arr can have size().");
    }

    if (type() == Obj)
    {
        return get_object().size();
    }
    else
    {
        return get_array().size();
    }
}

void Value::append(const Value &value)
{
    assert(type() == Arr);
    if (type() != Arr)
    {
        throw ValueException("Value Error: Only Arr can use append.");
    }

    Array &arr = get_array();
    // arr.resize(static_cast<int>(arr.size()) + 1);
    arr.resize(arr.size() + 1);
    operator[](-1) = value;
    operator[](-1).drop_key();
    operator[](-1).parent = this;
}

void Value::insert(const Key &key, const Value &value)
{
    assert(type() == Obj);
    if (type() != Obj)
    {
        throw ValueException("Value Error: Only Obj can use insert.");
    }

    operator[](key) = value;
}

void Value::drop(const Key &key)
{
    assert(type() == Obj);
    if (type() != Obj)
    {
        throw ValueException("Value Error: Only Obj can drop value by key");
    }

    Object &obj = get_object();
    if (obj.find(key) != obj.end())
    {
        obj.erase(key);
    }
}

void Value::drop(int idx)
{
    assert(type() == Arr);
    if (type() != Arr)
    {
        throw ValueException("Value Error: Only Arr can drop value by indx");
    }

    Array &arr = get_array();
    if (idx < 0)
    {
        idx = static_cast<int>(arr.size()) + idx;
    }

    assert(idx >= 0 && idx < static_cast<int>(arr.size()));
    if (idx < 0 || idx > static_cast<int>(arr.size()))
    {
        throw std::out_of_range("Invalid index.");
    }

    arr.erase(arr.begin() + idx);
}

void swap(Value &lhs, Value &rhs)
{
    std::swap(lhs._type, rhs._type);
    std::swap(lhs.val, rhs.val);
    std::swap(lhs.key, rhs.key);
    // std::swap(lhs.indent_step, rhs.indent_step);
    std::swap(lhs.parent, rhs.parent);
}

const Key &Value::get_key() const
{
    // assert(key != nullptr);
    if (key == nullptr)
    {
        throw ValueException("Value Error, can't get a empty key.");
    }
    return *key;
}

void Value::convert(ValueType value_type)
{
    if (value_type == type())
        return;

    switch (type())
    {
    case Obj:
        delete val.obj;
        break;
    case Arr:
        delete val.arr;
        break;
    case Str:
        delete val.str;
        break;
    default:
        break;
    }

    switch (value_type)
    {
    case Obj:
        val.obj = new Object();
        break;
    case Arr:
        val.arr = new Array();
        break;
    case Str:
        val.str = new String();
        break;
    case Num:
        val.num = 0;
        break;
    case Deci:
        val.deci = 0.0;
        break;
    case Bool:
        val.Bool = false;
        break;
    default:
        break;
    }

    _type = value_type;
}

Value::iterator Value::begin()
{
    ValueIter iter;
    iter.pos = this;
    iter.root = this;
    return iter;
}

Value::iterator Value::end()
{
    ValueIter iter;
    iter.pos = nullptr;
    iter.iter_data = true;
    iter.root = this;
    return iter;
}

Value::layer_iterator Value::layer_begin()
{
    layer_iterator iter;
    iter.root = this;
    if (type() == Obj)
    {
        iter.obj_iter = get_object().begin();
    }
    else if (type() == Arr)
    {
        iter.obj_iter = get_object().begin();
    }
    else
    {
        throw ValueException("Only Obj and Arr have layer_iterator.");
        
    }
    return iter;
}
Value::layer_iterator Value::layer_end()
{
    layer_iterator iter;
    iter.root = this;
    if (type() == Obj)
    {
        iter.obj_iter = get_object().end();
    }
    else if (type() == Arr)
    {
        iter.obj_iter = get_object().end();
    }
    else
    {
        throw ValueException("Only Obj and Arr have layer_iterator.");
    }
    return iter;
}