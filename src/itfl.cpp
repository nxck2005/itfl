// ITFL: tool to verify sha256 hash for files
// Simple tool to check file integrity manually.
// Author: Nishchal Ravi

// version 0.1.0

/*
    MIT License

    Copyright (c) 2025 Nishchal Ravi

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.

*/
#include "../lib/picosha2.h"
#include "../lib/cxxopts.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// Windows specific imports, for color right now
#ifdef _WIN32
#include <windows.h>
#include <io.h>
#define IS_TTY _isatty
#define FILENO _fileno
#else
#include <unistd.h>
#define IS_TTY isatty
#define FILENO fileno
#endif

// To manage terminal colors
class TerminalColor {
    public:
    const std::string red;
    const std::string green;
    const std::string blue;
    const std::string reset;

    // Set the ansi codes if the term isnt a TTY. Logic is in the private func
    TerminalColor() :
        red(colorSupport() ? "\033[31m" : ""),
        green(colorSupport() ? "\033[32m" : ""),
        blue(colorSupport() ? "\033[34m" : ""),
        reset(colorSupport() ? "\033[0m" : "")
    {}
    
    private:
    bool colorSupport() const {
        #ifdef _WIN32
        // For now, just use normal text if user is on Windows.
        // Yet to learn how windows handles it 
        return false;
        #else
        return IS_TTY(FILENO(stdout));
        #endif
    }
};

int main(int argc, char* argv[]) {
    // The second argument must be the filename, and third is the hash to check against. Parse it, and throw an error otherwise
    if (argc > 4 || argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <filename> <hash to check against> <-v>\n";
        return 1;
    }

    const TerminalColor color;

    bool verbose = false;
    
    const std::string filename = argv[1];
    const std::string givenHash = argv[2];

    if (argc > 3) {
        if (std::string(argv[3]) == "-v") verbose = true;
    }
    
    if (givenHash.length() != 64) {
        std::cerr << color.red << "Error: " << color.reset << "Invalid length for given hash string\n";
        return 1;
    }

    // Read the file's contents into a vector
    std::ifstream file_stream(filename, std::ios::binary);
    if (!file_stream) {
        std::cerr << color.red << "Error: " << color.reset << "Could not open file: '" << filename << "'.\n";
        return 1;
    }

    // Create a vector that'll store the hashed value in bytes (??)
    std::vector<unsigned char> s(picosha2::k_digest_size);
    picosha2::hash256(file_stream, s.begin(), s.end());

    // Bytes to the actual string
    std::string result = picosha2::bytes_to_hex_string(s.begin(), s.end());

    if (verbose) {
        std::cout << "Calculated SHA-256 hash of " << filename << ": " << result << std::endl;
        std::cout << "Given hash: " << givenHash << std::endl;
    }

    if (result == givenHash) {
        std::cout << color.green << "Hash check passed!" << color.reset << " Given file matches hash provided" << std::endl;
    } else {
        std::cout << color.red << "Hash check failed!" << color.reset << " File does not match hash provided" << std::endl;
    }
    return 0;
}