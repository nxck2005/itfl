# itfl: Is This File Legit
A lightweight command-line utility for SHA-256 file integrity verification.

## Description
```itfl``` provides a fast and simple way to verify that a file has not been corrupted during download or otherwise tampered with, by computing the SHA256 hash for the downloaded file, and comparing it with the hash for the original file.

Written in C++.

## Features
- __Fast Hashing__: Efficiently processes files of any size, thanks to the amazing single-header library by [Stephan Brumme](https://create.stephan-brumme.com/hash-library/)!
- __Simple__: Clean, single-purpose CLI tool that does one thing, and does it well.
- __No Dependencies__: No external libs required to compile and run.

## Installation
### From Source
### Prerequisites:
- A C++17 compliant compiler (gcc, clang, MSVC etc.)
- CMake (min. version: v3.24)
- Git

```bash
# Clone the repository
git clone https://github.com/your-username/itfl.git
cd itfl
mkdir build && cd build

# Compile
cmake ../
make

# Optional: Move into PATH for easy usage
# For Linux/macOS:
sudo mv itfl /usr/local/bin/

# For Windows, you can manually move itfl.exe to a folder in your %PATH%
```
### Binary Releases
I'll put out binaries for Linux for every major release :) x86_64 only.

## Usage

```bash
itfl <filename> <expected-sha256-hash> <--verbose>
```

### Arguments:
- ```filename``` : relative or absolute path to the file you want to verify
- ```expected-sha256-hash``` : SHA-256 hex string to check against
- ```--verbose``` : verbose flag; print out both computed and provided hash

More can be viewed by --help.

## Contributing

Contributions are welcome. Please fork the repo and use a feature branch if you wish to do so! Pull requests are welcome, too.

If you have a suggestion or a bug report, please open an issue [here.](https://github.com/nxck2005/itfl/issues "Issues")

## Short-term TODO
- Add a diff showing the differences in hashes if the hash check fails
- Calculate multiple hashes
- Scrape a hash from an endpoint? Hmm..

## Acknowledgements

This repo utilizes the single-header library by [Stephan Brumme](https://create.stephan-brumme.com/hash-library/), for the SHA-256 implementation, and [cxxopts](https://github.com/jarro2783/cxxopts) by [Jarryd Beck](https://github.com/jarro2783).

## License

This project is licensed under the __MIT License__. See the [LICENSE](https://github.com/nxck2005/itfl/blob/main/LICENSE "License for nxck2005/itfl") for more details.
