#ifndef PARSER_H
#define PARSER_H

#pragma once
#include "Value.h"
// #include <assert.h>
#include <exception>

class JSON SyntaxError : public std::exception
{
private:
    String error_info;
    String rls;
    int line;

public:
    const char *what() const noexcept override
    {
        
        return rls.data();
    }

    SyntaxError(int line, const String &info) : line(line), error_info(info) {
        rls = "In line " + std::to_string(line) + ", " + error_info;
    }
};

class JSON Parser
{
private:
    String text;
    Value root;
    int text_pos = -1;
    int line = 1;

    inline char next_char()
    {
        return text[text_pos + 1];
    }

    inline char now_char() {
        return text[text_pos];
    }

public:
    Parser() : text("\0") {};
    Parser(const String &text) : text(text + "\0"), text_pos(-1) {}


    // 以下解析函数最终都会使 text_pos 只想当前结构的最后一个字符

    /**
     * @brief 用于解码json中的字符串
     *  
     * @return String 
     */
    String decode_string();

    /**
     * @brief 跳过空白字符
     * 
     */
    void skip_blank();

    /**
     * @brief 解析 Value 对象
     * 
     * @return Value 
     */
    Value parser();

    /**
     * @brief 解析 json 中的字符串
     * 
     * @param value 
     */
    void parser_string(Value &value);

    /**
     * @brief 解析 json 中的对象
     * 
     * @param value 
     */
    void parser_object(Value &value);

    /**
     * @brief 解析 json 中的数组
     * 
     * @param value 
     */
    void parser_arr(Value &value);
    
    /**
     * @brief 解析 json 中的 null 和 bool 类型
     * 
     * @param value 
     */
    void parser_word(Value &value);

    /**
     * @brief 解析数字，目前支持十进制整数和十进制浮点数
     * 
     * @param value 
     */
    void parser_digit(Value &value);
};

#endif // PARSER_H