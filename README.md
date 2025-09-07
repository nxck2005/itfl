# itfl: Is This File Legit
A lightweight command-line utility for SHA-256 file integrity verification.

## Description
```itfl``` provides a fast and simple way to verify that a file has not been corrupted during download or otherwise tampered with, by computing the SHA256 hash for the downloaded file, and comparing it with the hash for the original file.

Written in C++.

## Features
- __Fast Hashing__: Efficiently processes files of any size, thanks to the amazing single-header library [picosha2](https://github.com/okdshin/PicoSHA2 "picosha2.h") by [okdshin](https://github.com/okdshin)!
- __Simple By Design:__: Clean, single-purpose CLI tool that does one thing, and does it well.
- __No Dependencies__: No external libs required to compile and run.

## Installation
### From Source
### Prerequisites:
- A C++11 compliant compiler (gcc, clang, MSVC etc.)
- Git

```bash
# Clone the repository
git clone https://github.com/your-username/itfl.git
cd itfl

# Compile
g++ -std=c++11 -O3 -o itfl src/itfl.cpp

# Optional: Move into PATH for easy usage
# For Linux/macOS:
sudo mv itfl /usr/local/bin/

# For Windows, you can manually move itfl.exe to a folder in your %PATH%
```
### Binary Releases
I'll put out binaries for Linux for every major release :) x86_64 only.

## Usage

```bash
itfl <filename> <expected-sha256-hash>
```

### Arguments:
- ```filename``` : relative or absolute path to the file you want to verify
- ```expected-sha256-hash``` : SHA-256 hex string to check against

### Successful Check:
```bash
$ itfl ./installer.zip e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855

Calculated SHA-256 hash of ./installer.zip: e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855
Given hash: e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855
FILE IS LEGIT!
```
### Failed Check:
```bash
$ itfl ./installer.zip e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855

Calculated SHA-256 hash of ./installer.zip: f2ca1bb6c7e907d06dafe4687e579fce76b37e4e93b7605022da52e6ccc26fd2
Given hash: e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855
HASH CHECK FAILED!
```
## Contributing

Contributions are welcome. Please fork the repo and use a feature branch if you wish to do so! Pull requests are welcome, too.

If you have a suggestion or a bug report, please open an issue [here.](https://github.com/nxck2005/itfl/issues "Issues")

## Acknowledgements

This repo utilizes the single-header library [picosha2](https://github.com/okdshin/PicoSHA2 "picosha2.h") by [okdshin](https://github.com/okdshin), for the SHA-256 implementation.

## License

This project is licensed under the __MIT License__. See the [LICENSE](https://github.com/nxck2005/itfl/blob/main/LICENSE "License for nxck2005/itfl") for more details.
