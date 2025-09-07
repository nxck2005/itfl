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

    std::vector<unsigned char> s(picosha2::k_digest_size);
    picosha2::hash256(file_stream, s.begin(), s.end());
    std::string result = picosha2::bytes_to_hex_string(s.begin(), s.end());

    std::cout << "SHA-256 hash of " << filename << ": " << result << std::endl;

    return 0;
}