#include <iostream>
#include <vector>
#include <utility>
#include <chrono>

int SearchFullScan(const std::string& text, const std::string& mask) {
    size_t t = 0;
    while (t <= text.size() - mask.size()) {
        size_t m = 0;
        while (m < mask.size() && text[t + m] == mask[m]) {
            m++;
        }
        if (m == mask.size()) {
            return t;
        }
        t++;
    }
    return -1;
};

int SearchFullScan2(const std::string& text, const std::string& mask) {
    size_t match = 0;
    size_t first_loc = 0;
    for (size_t i = 0; i < text.size(); ) {
        for (size_t j = 0; j < mask.size(); ) {
            if (text[i] == mask[j]) {
                match++;
                if (match == mask.size()) {
                    return first_loc;
                }
                j++;
                i++;
                continue;
            }
            first_loc++;
            i++;
        }
    }
    return -1;
};

int* CreateShift(const std::string& mask) {
    int shift[128];
    for (size_t i = 0; i < 128; i++) {
        shift[i] = mask.size();
    }
    for (size_t j = 0; j < mask.size(); j++) {
        shift[mask[j]] = 1;
    }
    return shift;
};

int SearchBM(const std::string& text, const std::string& mask) {
    int m = mask.size();
    int t = text.size();

    int* shift = CreateShift(mask);
    int s = 0;
    while (s <= (t - m)) {
        int j = m - 1;
        while (j >= 0 && mask[j] == text[s + j]) {
            j--;
        }

        if (j < 0) {
            return s;
        }
        else {
            s += std::max(1, j - shift[text[s + j]]);
        }
    }
};

int main()
{
    size_t size = 100000;
    std::vector<std::pair<std::string, std::string>> test = { { "AAABCABCD", "ABCD" }, { "AAABCABCD", "AA" }, { "AAABCABCD", "BCAB" }, { "AAABCABCD", "D" } };
    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < test.size(); j++) {
            int res = SearchBM(test[j].first, test[j].second);
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "SearchBM: " << duration.count() / size << " ms" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < test.size(); j++) {
            int res = SearchFullScan(test[j].first, test[j].second);
        }
    }
    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "SearchFullScan: " << duration.count() / size << " ms" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < test.size(); j++) {
            int res = SearchFullScan2(test[j].first, test[j].second);
        }
    }
    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "SearchFullScan2: " << duration.count() / size << " ms" << std::endl;
}
