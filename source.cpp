#include <iostream>
#include <string>
#include <vector>
#include <fstream>

/*
To compile and install the program on Debian, you can follow these steps:

1. Install the g++ compiler if it's not already installed. You can do this by running the following command in your terminal:
   sudo apt-get install g++

2. Navigate to the directory containing the source code file (let's call it base64.cpp) using the cd command.

3. Compile the source code file using the g++ compiler. You can do this by running the following command in your terminal:
   g++ -o base64 base64.cpp

4. Now, you can run the program using the following command:
   ./base64 --source_file "path_to_your_file" --destination_file "path_to_output_file"

Please replace "path_to_your_file" with the path to the file you want to convert to Base64, and "path_to_output_file" with the path where you want to save the Base64-encoded output. The program processes and writes each 3-byte chunk to the output file as soon as it's read.
*/

const std::string base64_chars = 
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

std::string base64_encode(const char* buffer, size_t size) {
    std::string ret;
    int val = 0;
    int valb = -6;
    for (size_t i = 0; i < size; ++i) {
        val = (val<<8) + buffer[i];
        valb += 8;
        while (valb >= 0) {
            ret.push_back(base64_chars[(val>>valb)&0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6) ret.push_back(base64_chars[((val<<8)>>(valb+8))&0x3F]);
    while (ret.size() % 4) ret.push_back('=');
    return ret;
}

int main(int argc, char* argv[]) {
    std::string source_file;
    std::string destination_file;
    for(int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if(arg == "--source_file" && i + 1 < argc) {
            source_file = argv[++i];
        } else if(arg == "--destination_file" && i + 1 < argc) {
            destination_file = argv[++i];
        }
    }
    std::ifstream file(source_file, std::ios::binary);
    if (!file) {
        std::cerr << "Could not open source file.\n";
        return 1;
    }
    std::ofstream outfile(destination_file);
    if (!outfile) {
        std::cerr << "Could not open destination file.\n";
        return 1;
    }
    char buffer[3];
    while (file.read(buffer, sizeof(buffer))) {
        std::string output = base64_encode(buffer, sizeof(buffer));
        outfile << output;
    }
    // Handle remaining bytes
    std::string output = base64_encode(buffer, file.gcount());
    outfile << output;

    return 0;
}