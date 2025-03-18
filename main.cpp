#include "json.h"
#include <fstream>
#include <sstream>
void OK()
{
    std::cout << "OK" << std::endl;
}

int main(int, char **)
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::ofstream os("out.json");

    set_indent(4);
    Reader reader;
    clock_t beg = clock();
    Value root = reader.read_from_file("C:\\Users\\foush\\code\\project\\json\\.vscode\\settings.json");
    for (auto &x : find(root.begin(), root.end(), "random"))
    {
        os << x->to_string() << '\n';
        // std::cout << std::endl;
        // it.next();
    }

    // for (auto it = root["user"].end();  it != root["user"].begin(); ) {
    //     --it;
    //     std::cout << (*it).to_string() << '\n';
    //     std::cout << std::endl;
    // }
    std::cout << clock() - beg << "ms\n";
}