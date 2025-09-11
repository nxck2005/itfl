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
#include "../lib/cxxopts.hpp"
#include "../lib/sha256.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <array>

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
        return false;
        #else
        return IS_TTY(FILENO(stdout));
        #endif
    }
};

// Given a file stream, return it's SHA256 hash using a buffer based approach
std::string getHash(std::ifstream& file_stream) {
    // Get a hasher object
    SHA256 sha256;

    // Buffer of size 256KB
    std::array<char, 262144> buf;

    // Read full buffers until eofbit and failbit are set by file_stream.read()
    // eofbit set when end of file is hit
    // This also triggers failbit because full read request was not completed
    // This will make the while loop evaluate to false
    // Process the read buffers
    std::streamsize bytesRead;
    while (file_stream.read(buf.data(), buf.size())) {
        bytesRead = file_stream.gcount();
        sha256.add(buf.begin() ,bytesRead);
    }

    // See if there were more than 0 bytes read at the end, when there's not a full buffer left
    // If so, process those bytes too
    bytesRead = file_stream.gcount();
    if (bytesRead > 0) {
        sha256.add(buf.begin(), bytesRead);
    }
    std::string computedHash = sha256.getHash();

    return computedHash;
}

int main(int argc, char* argv[]) {

    try {
        cxxopts::Options options("itfl", "A lightweight command-line utility for SHA-256 file integrity verification \n\nUsage:\n itfl [OPTIONS] <filename> <hash>");
        options.add_options()
            ("v,verbose", "Enable verbose output")
            ("f,filename", "File to process", cxxopts::value<std::string>())
            ("h,hash", "SHA-256 hash to check against", cxxopts::value<std::string>())
            ("version", "Print version information")
            ("help", "Print usage");

        // Filename and hash can be given anywhere without a flag, so parse them in this order
        options.parse_positional({"filename", "hash"});

        // Return an object that contains proxy objects for the arguments
        auto result = options.parse(argc, argv);

        const TerminalColor color;

        // .count() returns number of times the option was specified
        // 1 would evaluate this to true
        if (result.count("help")) {
            std::cout << options.help() << std::endl;
            return 0;
        }

        if (result.count("version")) {
            // TODO: grab from cmake
            std::cout << "itfl version " << "0.2.0" << std::endl;
            return 0;
        }

        if (result.count("filename") == 0 || result.count("hash") == 0) {
            std::cerr << color.red << "Error: " << color.reset << "Missing required arguments. \n\n" << options.help() << std::endl;
            return 1;
        }

        // 1 evaluates to true
        bool verbose = result.count("verbose");

        // Cast the proxy object value to an actual string
        const std::string filename = result["filename"].as<std::string>();
        const std::string givenHash = result["hash"].as<std::string>();
        
        if (givenHash.length() != 64) {
            std::cerr << color.red << "Error: " << color.reset << "Invalid length for given hash string\n";
            return 1;
        }

        // Read the file's contents in binary mode
        std::ifstream file_stream(filename, std::ios::binary);
        if (!file_stream) {
            std::cerr << color.red << "Error: " << color.reset << "Could not open file: '" << filename << "'.\n";
            return 1;
        }

        // This vector based approach is inefficient. Just hashing a 1.5Gig file scaled to 1MB :/
        // TODO: switch to a buffer based approach

        // Create a vector that'll store the hashed value in bytes (??)
        // std::vector<unsigned char> s(picosha2::k_digest_size);
        // picosha2::hash256(file_stream, s.begin(), s.end());

        // // Bytes to the actual string
        // std::string computedHash = picosha2::bytes_to_hex_string(s.begin(), s.end());

        std::string computedHash = getHash(file_stream);

        if (verbose) {
            std::cout << "Calculated SHA-256 hash of " << filename << ": " << computedHash << std::endl;
            std::cout << "Given hash: " << givenHash << std::endl;
        }

        if (computedHash == givenHash) {
            std::cout << color.green << "Hash check passed!" << color.reset << " Given file matches hash provided" << std::endl;
        } else {
            std::cout << color.red << "Hash check failed!" << color.reset << " File does not match hash provided" << std::endl;
        }
    } catch (const cxxopts::exceptions::exception& e) {
        // If anything occurs, deviate from the happy little path
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}