#include "Reader.h"
#include <fstream>
#include <sstream>

Value Reader::read()
{
    try
    {
        Parser parser(text);
        return parser.parser();
    }
    catch (const SyntaxError &syntax_err)
    {
        std::cout << "\033[31mJSON Syntax Error: \033[0m" << syntax_err.what() << std::endl;
        return Value();
    }
}

Value Reader::read_from_text(const String &text)
{
    this->text = text;
    return read();
}

Value Reader::read_from_file(const String &path)
{
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    text = buffer.str();
    return read();
}