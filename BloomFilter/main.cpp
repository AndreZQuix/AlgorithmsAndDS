#include "BloomFilter.h"
#include <fstream>
#include <iostream>
#include <string>

int main() {
	BloomFilter f;
	std::ifstream infile("words2.txt");
	std::string word;
	while (std::getline(infile, word)) {
		f.insert(word);
	}
	infile.close();

	std::ifstream ifs("words2.txt");
	size_t words_count = 0;
	size_t positive = 0;
	size_t false_positive = 0;

	while (std::getline(ifs, word)) {
		words_count++;
		bool b = f.contains(word);
		if (b) {
			positive++;
		}
	}
	
	std::ifstream ifs2("words.txt");
	while (std::getline(ifs2, word)) {
		bool b = f.contains(word);
		if (b) {
			false_positive++;
		}
	}

	int accuracy = (((double)positive - (double)false_positive) / (double)words_count) * 100;
	std::cout << "Words count: " << words_count << " Positive: " << positive << " False positive: " << false_positive << " Accuracy: " << accuracy << "%" << std::endl;
};