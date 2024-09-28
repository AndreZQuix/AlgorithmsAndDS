#include <iostream>
#include "Barn.h"

int main()
{
    Barn barn;
    barn.generate_map();
    barn.print();
    size_t solution = barn.solve();
    std::cout << solution << std::endl;
}

