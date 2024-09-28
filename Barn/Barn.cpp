#include <iostream>
#include "Barn.h"

int main()
{
    size_t N = 5;
    size_t seed = 123456;
    Barn barn = Barn(N);
    barn.generate_map(seed);
    barn.print();
    size_t solution = barn.solve();
    std::cout << solution << std::endl;
}

