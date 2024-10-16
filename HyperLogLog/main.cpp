#include <iostream>
#include <random>
#include "HyperLogLog.h"

const size_t BITSET_SIZE = 32;

size_t generate_random_bitsets(std::vector<std::bitset<BITSET_SIZE>>& vec, size_t size) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(1, 1000000);
    for (size_t i = 0; i < size; i++) {
        vec.push_back(std::bitset<BITSET_SIZE>(dist(rng)));
    }
    std::cout << "Vector of random bitsets has been generated \n";
    std::vector<std::bitset<BITSET_SIZE>> v = vec;
    std::sort(v.begin(), v.end(), [](const auto& lhs, const auto& rhs)
        { return lhs.to_string() < rhs.to_string(); });

    size_t unique_count = std::unique(v.begin(), v.end()) - v.begin();
    std::cout << "Unique count number has been calculated \n";
    return unique_count;
};

int main()
{
    std::vector<size_t> sizes = { 10000, 100000, 1000000 };
    for (const auto size : sizes) {
        std::cout << "Size is " << size << ": \n";
        for (size_t i = 0; i < 5; i++) {
            std::vector<std::bitset<BITSET_SIZE>> bitsets;
            size_t unique_nums = generate_random_bitsets(bitsets, size);
            std::cout << "HLL start \n";
            HyperLogLog hll(512);
            for (const auto& b : bitsets) {
                hll.add(b);
            };

            unsigned long hll_count = hll.count();
            double missed = std::abs((((double)unique_nums / (double)hll_count) * 100) - 100);
            std::cout << "Amount: " << bitsets.size() << " Unique: " << unique_nums << " HLL count: " << hll_count << " Missed: " << missed << "%" << " Size: " << hll.size() << " bytes \n\n";
        }
        std::cout << std::endl;
    }
}
