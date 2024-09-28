#pragma once

#include <vector>
#include <iostream>

class Barn {
	size_t N;
	std::vector<std::vector<bool>> map;

	size_t find_height(size_t i, size_t j, size_t limit_height) {
		size_t height = 0;
		while (height <= limit_height && j - height >= 0 && j - height < map.size() && !map[i][j - height]) {
			height++;
		}
		return height;
	};

	size_t find_max_square(size_t i, size_t j) {
		size_t max_square = 0;
		size_t limit_height = N;
		for (size_t width = 1; i + width - 1 < map.size(); width++) {
			size_t height = find_height(i + width - 1, j, limit_height);
			if (limit_height > height) {
				limit_height = height;
			}
			size_t square = width * limit_height;
			if (max_square < square) {
				max_square = square;
			}
		}
		return max_square;
	};

public:
	Barn() { };

	Barn(size_t N) : N(N) {
		map.resize(N);
		for (size_t i = 0; i < N; i++) {
			map[i].resize(N);
		}
	};

	void generate_map(size_t seed) {
		srand(seed);

		for (size_t i = 0; i < map.size(); i++) {
			for (size_t j = 0; j < map.size(); j++) {
				map[i][j] = 0 + (rand() % (1 - 0 + 1)) == 1;
			}
		}
	};

	void generate_map() {
		std::cin >> N;
		map.resize(N);
		for (size_t i = 0; i < map.size(); i++) {
			for (size_t j = 0; j < map.size(); j++) {
				map[i].resize(N);
				size_t num;
				std::cin >> num;
				if (num != 0 && num != 1) {
					std::cout << "incorrect data";
					return;
				}

				map[i][j] = num;
			}
		}

		
	};

	void print() {
		for (size_t i = 0; i < map.size(); i++) {
			for (size_t j = 0; j < map.size(); j++) {
				std::cout << (map[i][j] ? "1 " : ". ");
			}
			std::cout << '\n';
		}
	};

	size_t solve() {
		size_t max_square = 0;
		for (size_t i = 0; i < map.size(); i++) {
			for (size_t j = 0; j < map.size(); j++) {
				size_t square = find_max_square(i, j);
				if (max_square < square) {
					max_square = square;
				}
			}
		}
		return max_square;
	};
};