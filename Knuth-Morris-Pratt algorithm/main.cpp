#include <string>
#include <iostream>
#include <vector>
#include <chrono>

class Auto {
	std::string alphabet;
	size_t length;
	std::vector<std::vector<size_t>> delta;

	std::string Left(const std::string& pattern, size_t amount) {
		return pattern.substr(0, amount);
	};

	std::string Right(const std::string& pattern, size_t amount) {
		return pattern.substr(pattern.size() - amount);
	};

	void CreateDelta(const std::string& pattern) {
		for (size_t q = 0; q <= length; q++) {
			for (char c : alphabet) {
				int k = q + 1;
				if (q == length) {
					k--;
				}

				std::string line = Left(pattern, q) + c;
				while (Left(pattern, k) != Right(line, k)) {
					k--;
				}
				delta[q][c - alphabet[0]] = k;
			}
		}
	};

public:
	Auto(const std::string& pattern, const std::string& alphabet) {
		this->alphabet = alphabet;
		length = pattern.size();
		delta = std::vector<std::vector<size_t>>(length + 1);
		for (size_t i = 0; i < delta.size(); i++) {
			delta[i].resize(alphabet.size());
		}
		CreateDelta(pattern);
	};

	size_t Search(const std::string& text) {
		int q = 0;
		for (size_t i = 0; i < text.size(); i++) {
			q = delta[q][text[i] - alphabet[0]];
			if (q == length) {
				return i - length + 1;
			}
		}
		return -1;
	};
};

class KMP {
	std::vector<size_t> pi;
	std::string pattern;

public:
	KMP(const std::string& pattern) : pattern(pattern) {
		pi = std::vector<size_t>(pattern.size() + 1);
		pi[0] = pi[1] = 0;
		for (int q = 1; q < pattern.size(); q++) {
			int len = pi[q];
			while (len > 0 && pattern[len] != pattern[q]) {
				len = pi[len];
			}

			if (pattern[len] == pattern[q]) {
				len++;
			}
			pi[q + 1] = len;
		}
	};

	size_t Search(const std::string& text) {
		for (size_t k = 0, i = 0; i < text.length(); ++i)
		{
			while ((k > 0) && (pattern[k] != text[i])) {
				k = pi[k - 1];
			}

			k++;

			if (k == pattern.length()) {
				return (i - pattern.length() + 1);
			}

		}
	};
};

int main() {
	size_t size = 10000000;

	Auto autom("ABABC", "ABC");
	auto start = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < size; i++) {
		size_t res = autom.Search("AABABABABC");
	}
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	std::cout << "Auto dur: " << duration.count() / (size / 10) << " ms" << std::endl;

	KMP kmp = KMP("ABABC");
	start = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < size; i++) {
		size_t res = kmp.Search("AABABABABC");
	}
	stop = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	std::cout << "KMP dur: " << duration.count() / (size / 10) << " ms" << std::endl;

	return 0;
};