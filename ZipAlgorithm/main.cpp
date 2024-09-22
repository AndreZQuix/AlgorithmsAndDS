#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

std::string RLE_string(const std::string& text) {
    std::string result;
    size_t count = 0;
    for (size_t i = 0; i < text.size(); i++) {
        if (text[i] == text[i + 1]) {
            count++;
        }
        else {
            count++;
            result += std::to_string(count) + ' ' + text[i] + ' ';
            count = 0;
        }
    }
    return result;
};

std::string RLE_unpack_string(const std::string& text) {
    std::istringstream iss(text);
    std::string temp;
    std::vector<std::string> chars;
    while (std::getline(iss, temp, ' ')) {
        chars.push_back(temp);
    }

    std::string result;
    for (size_t i = 0; i < chars.size(); i += 2) {
        size_t count = std::stoi(chars[i]);
        for (size_t j = 0; j < count; j++) {
            result += chars[i + 1];
        }
    }
    return result;
};

std::string GetNewFileName(const std::string& file_path, bool is_packed) {
    size_t slash_pos = file_path.find_last_of('/');
    if (slash_pos == std::string::npos) {
        slash_pos = file_path.find_last_of("\\");
    }
    if (slash_pos == std::string::npos) {
        std::cout << "Incorrect file name" << std::endl;
        return std::string();
    }

    std::string old_name = file_path.substr(slash_pos, file_path.size());
    std::string new_name = old_name + (is_packed ? "_packed" : "_unpacked");
    std::string result = file_path.substr(0, slash_pos);
    result += new_name;
    return result;
};

std::string RLE_file(const std::string& file_path) {
    std::string result = GetNewFileName(file_path, true);
    std::ifstream t(file_path, std::ios::in | std::ios::binary);
    std::stringstream buffer;
    buffer << t.rdbuf();
    std::string packed = RLE_string(buffer.str());
    std::ofstream ofs(result, std::ios::out | std::ios::binary);
    ofs << packed;
    return result;
};

std::string RLE_unpack_file(const std::string& file_path) {
    std::string result = GetNewFileName(file_path, false);
    std::ifstream t(file_path, std::ios::in | std::ios::binary);
    std::stringstream buffer;
    buffer << t.rdbuf();
    std::string unpacked = RLE_unpack_string(buffer.str());
    std::ofstream ofs(result, std::ios::out | std::ios::binary);
    ofs << unpacked;
    return result;
};

int HandleStringPack(int argc, char** argv) {
    if (argc == 3 && (strcmp(argv[1], "--pack-string") == 0 || strcmp(argv[1], "-ps") == 0))
    {
        std::string zip = RLE_string(argv[2]);
        std::cout << zip << std::endl;
        return 0;
    }

    if (argc == 3 && (strcmp(argv[1], "--unpack-string") == 0 || strcmp(argv[1], "--ups") == 0))
    {
        std::string unzip = RLE_unpack_string(argv[2]);
        std::cout << unzip << std::endl;
        return 0;
    }

    return 1;
};

int HandleFile(int argc, char** argv) {
    if (argc == 3 && (strcmp(argv[1], "--pack-file") == 0 || strcmp(argv[1], "-pf") == 0))
    {
        std::string zip = RLE_file(argv[2]);
        if (zip != "") {
            std::cout << zip << std::endl;
            return 0;
        }
        return 1;
    }

    if (argc == 3 && (strcmp(argv[1], "--unpack-file") == 0 || strcmp(argv[1], "-upf") == 0))
    {
        std::string unzip = RLE_unpack_file(argv[2]);
        if (unzip != "") {
            std::cout << unzip << std::endl;
            return 0;
        }
        return 1;
    }

    return 1;
};

int main(int argc, char** argv)
{
    if (argc == 2 && strcmp(argv[1], "--help") == 0)
    {
        std::cout << "Commands: \n" << "--pack-string -ps Pack string" << "\n" << "--unpack-string -ups Unpack string" << std::endl;
        std::cout << "--pack-file -pf Pack file (Absolute path only)" << "\n" << "--unpack-file -upf Unpack file (Absolute path only)" << std::endl;
        return 0;
    }

    HandleStringPack(argc, argv);
    HandleFile(argc, argv);
}
