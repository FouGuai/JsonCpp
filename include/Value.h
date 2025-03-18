#ifndef VALUE_H
#define VALUE_H

#pragma once

#include "config.h"
#include "Type.h"
#include "ValueIter.h"
#include <iostream>

class JSON ValueException : public std::exception
{
public:
    String err_info;
    const char *what() const noexcept override
    {
        return err_info.data();
    }

    ValueException(const String &str) : err_info(str) {}
};

/**
 * @brief ValueType represent the type of Value.
 * For a Value Object, you can use obj.type() or type(obj) to get this.
 */
enum JSON ValueType
{
    Null,
    Obj,
    Arr,
    Str,
    Num,
    Deci,
    Bool,
};

class JSON Value
{

    friend void swap(Value &lhs, Value &rhs);

private:
    /**
     * @brief the type of Value, use type(obj) or obj.type() to get it.
     *
     */
    ValueType _type;

    /**
     * @brief The content of Value.
     *
     */
    union
    {
        String *str;
        Number num;
        double deci;
        bool Bool;
        Array *arr;
        Object *obj;
    } val;

    /**
     * @brief
     * key 中存储该对象的键值
     */
    Key *key = nullptr;
    Value *parent = nullptr;
    // int indent_step = 2;

    /**
     * @brief this->type() == Obj 时, 可以用来访问其中的键值对，
     * 如果不存在该键则会自动创建一个类型为 Null 的值
     *
     * @param key
     * @return Value&
     */

    // const Value &operator[](const Key &key) const;

    void delete_value();
    void fix_parent();

public:
    using iterator = ValueIter;
    using layer_iterator = ValueLayerIter;

    ValueType type() const;

#pragma region constructor
    // 默认构造函数，默认为Null类型
    Value() : _type(Null) {}

    /**
     * @brief 根据类型构造Value对象，并初始化为默认值
     *
     * @param t
     */
    explicit Value(ValueType t);

    /**
     * @brief Construct a new Value object
     *
     * @param str copy的值
     * @param copy true代表复制str的值
     */
    explicit Value(String *str);

    /**
     * @brief Construct a new Value object
     *
     * @param arr
     * @param copy
     */

    Value(Array *arr);
    Value(Object *obj);
    explicit Value(double deci);
    explicit Value(Number num);
    explicit Value(bool x);

    // 使用引用传递的构造函数，全部都为复制
    explicit Value(const String &str);
    explicit Value(const Array &arr);
    explicit Value(const Object &obj);
    explicit Value(const char *str);

    // 带键值的构造函数
    explicit Value(const Key &key, double deci);
    explicit Value(const Key &key, Number num);
    explicit Value(const Key &key, bool x);

    // 带键值, 使用引用传递的构造函数，全部都为复制
    explicit Value(const Key &key, Array *arr);
    explicit Value(const Key &key, Object *obj);
    explicit Value(const Key &key, const String &str);
    explicit Value(const Key &key, const Array &arr);
    explicit Value(const Key &key, const Object &obj);
    explicit Value(const Key &key, const char *str);

    Value(const Value &rhs);
    Value(Value &&rhs);

    /**
     * @brief 赋值运算符，注意赋值运算符不改变原对象的键值
     *
     * @param rhs
     * @return Value&
     */
    Value &operator=(Value rhs);

    ~Value();
#pragma endregion

    Value &find_first(const Key &key);
    std::vector<Value> find_all(const Key &Key) const;
    std::vector<Value> find_by_pattern(const std::string &pattern);

#pragma region get_set
    String &get_string()
    {
        return *val.str;
    }

    Object &get_object()
    {
        return *val.obj;
    }

    Number &get_num()
    {
        return val.num;
    }

    double &get_deci()
    {
        return val.deci;
    }

    Array &get_array()
    {
        return *val.arr;
    }

    bool &get_bool()
    {
        return val.Bool;
    }

    const String &get_string() const
    {
        return *val.str;
    }

    const Object &get_object() const
    {
        return *val.obj;
    }

    Number get_num() const
    {
        return val.num;
    }

    double get_deci() const
    {
        return val.deci;
    }

    const Array &get_array() const
    {
        return *val.arr;
    }

    bool get_bool() const
    {
        return val.Bool;
    }

    Value *get_parent() const
    {
        return parent;
    }
#pragma endregion

    /**
     * @brief 设置对象的键值 （_have_key 和 key 字段）
     *
     * @param key
     */
    void set_key(const Key &key);

    /**
     * @brief 删除该对象的键值，（!! WARING !! 此函数在json树中使用可能会使输出出错）
     *
     */
    void drop_key();

    /**
     * @brief 获得 key 对象， 如果 key 为空则抛出异常
     *
     * @return const Key&
     */
    const Key &get_key() const;

    /**
     * @brief 该对象是否有键值
     *
     * @return true
     * @return false
     */
    bool have_key() const
    {
        return this->key != nullptr;
    }

    /**
     * @brief 将Value对象转化成json文本格式
     *
     * @param str 存储的字符串
     * @param indent 基础缩进
     */
    void to_string(String &str, int indent = 0) const;

    /**
     * @brief 将Value对象转化成json文本格式
     *
     * @param str
     * @param indent
     */
    void to_string(String *str, int indent = 0) const;

    String to_string(int indent = 0) const;
    /**
     * @brief 当类型为 Obj 时，根据键 key 获得值
     *
     * @param key
     * @return Value&
     */
    Value &operator[](const Key &key);

    /**
     * @brief 当类型为 Arr 时，根据数组下标 idx 获得值
     *
     * @param idx
     * @return Value&
     */
    Value &operator[](int idx);
    const Value &operator[](int idx) const;
    /**
     * @brief 当类型为 Arr 或 Obj 时, 获得其中存储值的个数
     *
     */
    size_t size() const;

    /**
     * @brief 当类型为 Arr 时，此函数向数组最后添加一个元素
     *
     * @param value
     */
    void append(const Value &value);

    /**
     * @brief 当类型为 Obj 时， 删除键为 key 的元素
     *
     * @param key
     */
    void drop(const Key &key);

    /**
     * @brief 当类型为 Arr 时， 删除下标为 idx 的元素
     *
     * @param idx
     */
    void drop(int idx);

    /**
     * @brief 当类型为 Obj 时， 在键值 key 中插入元素（会覆盖原有的元素）
     *
     * @param key
     * @param value
     */
    void insert(const Key &key, const Value &value);

    /**
     * @brief 将类型转换为指定类型
     *
     * @param value_type
     */
    void convert(ValueType value_type);

    iterator begin();
    iterator end();
    layer_iterator layer_begin();
    layer_iterator layer_end();
};

// 定义Null
#define NULL_VALUE Value()

/**
 * @brief 用于获取对象的类型的api
 *        需要定义 obj.type()
 * @tparam T
 * @param value
 * @return decltype(value.type())
 */
template <typename T>
auto JSON type(const T &value) -> decltype(value.type())
{
    return value.type();
}

template <typename T>
auto JSON len(const T &val) -> decltype(val.size())
{
    return val.size();
}
#endif // VALUE_H