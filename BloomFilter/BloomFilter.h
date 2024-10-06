#pragma once

#include <vector>
#include <stdexcept>
#include <memory>
#include <bitset>
#include <random>

class BloomFilter {
	const size_t hash_func_count;
	std::vector<int> hash_seeds;

	std::vector<int> MD5_hash_results_buffer;
	static constexpr size_t bloomfilter_store_size = UINT32_MAX;
	std::vector<bool> bloom_filter_store;

	void hash(const std::string& object_as_string) {
		for (size_t i = 0; i < hash_func_count; i++) {
			int hash = hash_seeds[i];
			for (size_t i = 0; i < object_as_string.size(); i++)
			{
				hash ^= ((hash << 5) + object_as_string[i] + (hash >> 2));
			}

			MD5_hash_results_buffer[i] = (hash & 0x7FFFFFFF);
		}
	};

	void clear() {
		MD5_hash_results_buffer.clear();
		bloom_filter_store.clear();
	};

public:
	BloomFilter(size_t hash_func_count = 4) : hash_func_count(hash_func_count) {
		if (hash_func_count == 0) {
			throw std::invalid_argument("Hash functions count can not be equal to 0");
		}

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distr(0, INT32_MAX);
		for (size_t i = 0; i < hash_func_count; i++) {
			hash_seeds.push_back(distr(gen));
		}

		MD5_hash_results_buffer.resize(hash_func_count);
		bloom_filter_store.resize(bloomfilter_store_size, false);
	};

	bool contains(const std::string& object) {
		hash(object);

		for (size_t i = 0; i < hash_func_count; i++) {
			const int index_to_get = MD5_hash_results_buffer[i];
			if (!bloom_filter_store[index_to_get]) {
				return false;
			}
		}
		return true;
	};

	void insert(const std::string& object) {
		hash(object);

		for (size_t i = 0; i < hash_func_count; i++) {
			const int index_to_set = MD5_hash_results_buffer[i];
			bloom_filter_store[index_to_set] = true;
		}
	};
};