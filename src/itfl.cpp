#include "../lib/picosha2.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    const std::string filename = argv[1];

    // Read the file's contents into a vector
    std::ifstream file_stream(filename, std::ios::binary);
    if (!file_stream) {
        std::cerr << "Error: Could not open file " << filename << ".\n";
        return 1;
    }

    std::vector<char> file_data((std::istreambuf_iterator<char>(file_stream)),
                                 std::istreambuf_iterator<char>());

    // Now, pass the data container to the picosha2 hash function
    std::string hash_hex_str;
    picosha2::hash256(file_data, hash_hex_str);

    std::cout << "SHA-256 hash of " << filename << ": " << hash_hex_str << std::endl;

    return 0;
}