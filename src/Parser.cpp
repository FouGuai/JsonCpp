#include "Parser.h"
#include <cmath>

void Parser::skip_blank()
{
    while (isspace(text[text_pos + 1]) && text_pos + 1 < text.size())
    {
        if (text_pos >= 0 && text[text_pos] == '\n')
        {
            ++line;
        }
        text_pos++;
    }
}

Value Parser::parser()
{

    skip_blank();
    char next = next_char();
    if (next == '{')
    {
        Value value(Obj);
        parser_object(value);
        return value;
    }
    else if (next == '[')
    {
        Value value(Arr);
        parser_arr(value);
        return value;
    }
    else if (next == '"')
    {
        Value value(Str);
        parser_string(value);
        return value;
    }
    else if (isdigit(next) || next == '-')
    {
        Value value(Num);
        parser_digit(value);
        return value;
    }
    else if (isalpha(next))
    {
        Value value(Null);
        parser_word(value);
        return value;
    }
    else
    {
        return Value();
    }
}

// { key : Value, key : Value, ..., key : Value}
void Parser::parser_object(Value &value)
{
    ++text_pos;
    while (1)
    {
        skip_blank();

        if (next_char() == '}')
            break;
        if (next_char() != '"')
            throw SyntaxError(line, "need a key");

        String key = decode_string();

        skip_blank();

        if (next_char() != ':')
            throw SyntaxError(line, "need a ':'.");
        ++text_pos;

        Value val = parser();
        value[key] = val;

        skip_blank();

        if (next_char() == ',')
        {
            ++text_pos;
            continue;
        }
        else if (next_char() == '}')
        {
            break;
        }
        else
        {
            String info = "Unknown symbol: ";
            info.push_back(next_char());
            info.push_back('.');
            throw SyntaxError(line, info);
        }
    }
    ++text_pos;
}

void Parser::parser_string(Value &value)
{
    String str = decode_string();
    value.get_string() = str;
}

String Parser::decode_string()
{

    if (next_char() != '"')
    {
        throw SyntaxError(line, "Need a string");
    }

    ++text_pos;
    String str;

    while (1)
    {
        if (next_char() == '"')
        {
            break;
        }

        else if (next_char() == '\\')
        {
            ++text_pos;
            char nc = next_char();
            switch (nc)
            {
            case 'b':
                str.push_back('\b');
                break;
            case 'f':
                str.push_back('\f');
                break;
            case 't':
                str.push_back('\t');
                break;
            case 'n':
                str.push_back('\n');
                break;
            case 'r':
                str.push_back('\r');
                break;
            // Need \u841
            default:
                str.push_back(nc);
                break;
            }
            // ++text_pos;
        }
        else if (next_char() == '\0')
        {
            throw SyntaxError(line, "string are not closed");
        }
        else
        {
            str.push_back(next_char());
        }
        ++text_pos;
    }
    ++text_pos;
    return str;
}

// [Value, Value, ...]
void Parser::parser_arr(Value &value)
{
    ++text_pos;
    while (1)
    {
        skip_blank();

        if (next_char() == ']')
            break;

        Value val = parser();
        value.append(val);

        skip_blank();

        if (next_char() == ',')
        {
            ++text_pos;
            continue;
        }
        else if (next_char() == ']')
        {
            break;
        }
        else
        {
            String info = "Unknown symbol: ";
            info.push_back(next_char());
            info.push_back('.');
            throw SyntaxError(line, info);
        }
    }

    ++text_pos;
}

void Parser::parser_digit(Value &value)
{
    Number num = 0;
    double dot_num = 0.0;
    double devicer = 10.0;
    int state = 0;
    bool negative = false;
    bool exp_negative = false;
    Number exp_num = 0;

    while (1)
    {
        char nc = next_char();
        switch (state)
        {
        case 0:
        {
            if (nc == '-')
            {
                state = 1;
                negative = true;
            }
            else if (isdigit(nc))
            {
                num = num * 10 + nc - '0';
                state = 2;
            }
        }
        break;

        case 1:
        {
            if (isdigit(nc))
            {
                num = num * 10 + nc - '0';
                state = 2;
            }
            else
            {
                throw SyntaxError(line, "After '-' need a digit at least.");
            }
        }
        break;

        case 2:
        {
            if (nc == '.')
            {
                state = 3;
            }
            else if (isdigit(nc))
            {
                num = num * 10 + nc - '0';
            }
            else if (nc == 'e')
            {
                state = 5;
            }
            else
            {
                value.convert(Num);
                value.get_num() = negative ? -num : num;
                // ++text_pos;
                return;
            }
        }
        break;

        case 3:
        {
            if (isdigit(nc))
            {
                dot_num += (nc - '0') / devicer;
                devicer *= 10;
                state = 4;
            }
            else
            {
                throw SyntaxError(line, "after '.' need a digit at least.");
            }
        }
        break;

        case 4:
        {
            if (isdigit(nc))
            {
                dot_num += (nc - '0') / devicer;
                devicer *= 10;
            }
            else if (nc == 'e')
            {
                state = 5;
            }
            else
            {
                double rls = (num + dot_num) * (negative ? -1 : 1);
                value.convert(Deci);
                value.get_deci() = rls;
                // ++text_pos;
                return;
            }
        }
        break;
        case 5:
        {
            if (isdigit(nc))
            {
                exp_num = exp_num * 10 + nc - '0';
                state = 7;
            }
            else if (nc == '-')
            {
                exp_negative = true;
                state = 6;
            }
            else
            {
                throw SyntaxError(line, "after 'e' need a digit at least.");
            }
        }
        break;
        case 6:
        {
            if (isdigit(nc))
            {
                exp_num = exp_num * 10 + nc - '0';
                state = 7;
            }
            else
            {
                throw SyntaxError(line, "After '-' need a digit at least.");
            }
        }
        break;
        case 7:
        {
            if (isdigit(nc))
            {
                exp_num = exp_num * 10 + nc - '0';
            }
            else
            {
                double rls = (num + dot_num) * (negative ? -1 : 1);
                rls *= std::pow(10, (exp_negative ? -exp_num : exp_num));
                value.convert(Deci);
                value.get_deci() = rls;
                // ++text_pos;
                return;
            }
        }
        break;
        default:
        {
            throw std::bad_alloc();
            break;
        }
        }
        ++text_pos;
    }
}

void Parser::parser_word(Value &value)
{
    String str;
    while (isalpha(next_char()))
    {
        str.push_back(next_char());
        ++text_pos;
    }

    if (str == "true")
    {
        value.convert(Bool);
        value.get_bool() = true;
    }
    else if (str == "false")
    {
        value.convert(Bool);
        value.get_bool() = false;
    }
    else if (str == "null")
    {
        return;
    }
    else
    {
        String info = "Unknown symbol: ";
        info += str + ".";
        // info.push_back('.');
        throw SyntaxError(line, info);
        // throw
    }
}