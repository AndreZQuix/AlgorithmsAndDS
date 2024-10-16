#pragma once

#include <bit>
#include <string>
#include <vector>
#include <bitset>
#include <math.h>

struct HyperLogLog {
	static constexpr size_t BITSET_SIZE = 32;

	std::vector<int> registers;
	unsigned int m;
	unsigned int b;
	double alpha;

	HyperLogLog(unsigned int registers_count) : m(registers_count) {
		registers.resize(registers_count);
		b = static_cast<unsigned int>(ceil(log2(static_cast<double>(registers_count))));
		if (m <= 16) {
			alpha = 0.673;
		}
		else if (m <= 32) {
			alpha = 0.697;
		}
		else if (m <= 64) {
			alpha = 0.709;
		}
		else {
			alpha = 0.7213 / (1 + 1.079 / m);
		}
	};

	int leftmost_active_bit(size_t x) {
		return 1 + std::countl_zero(x);
	};

	size_t create_hash(std::bitset<BITSET_SIZE> bytes) {
		return std::hash<std::bitset<BITSET_SIZE>>{}(bytes);
	};

	unsigned long long size() {
		return sizeof(HyperLogLog) + (sizeof(int) * registers.size());
	};

	void add(std::bitset<BITSET_SIZE> data) {
		size_t hash = create_hash(data);
		int bit = leftmost_active_bit(hash << b);
		size_t shift = hash >> (m - b);

		if (bit > registers[shift]) {
			registers[shift] = bit;
		}
	};

	unsigned long count() {
		double sum = 0;
		double reg_count = static_cast<double>(m);
		for (const auto& r : registers) {
			double double_r = static_cast<double>(r);
			double double_r_pow = pow(2, double_r);
			sum += pow(double_r_pow, -1);
		}
		double estimate = (alpha * reg_count * reg_count / sum);

		return static_cast<unsigned int>(estimate);
	};
};