#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <sstream>
#include <cstdint>

void append_text_to_file(const std::string& filename, const std::string& text) {
    std::ofstream output(filename, std::ios::binary | std::ios::app);
    if (!output) {
        throw std::runtime_error("Cannot open output file");
    }

    output << '\n' << text;
    output.close();
}

std::string read_text_from_end_of_file(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) {
        throw std::runtime_error("Cannot open input file");
    }

    std::streampos file_size = file.tellg();
    file.seekg(-1, std::ios::cur);

    std::string text;
    char ch;
    while (file.tellg() > 0) {
        file.get(ch);
        if (ch == '\n' || ch == '\r') {
            break;
        }
        file.seekg(-2, std::ios::cur);
    }

    if (ch == '\r' && file.tellg() < file_size.operator-(1)) {
        file.get(ch);
        if (ch != '\n') {
            file.seekg(-1, std::ios::cur);
        }
    }

    std::getline(file, text);
    file.close();

    return text;
}

void remove_custom_metadata(const std::string& filename) {
    const std::vector<uint8_t> iend_chunk = {0x49, 0x45, 0x4E, 0x44}; // IEND in ASCII
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) {
        throw std::runtime_error("Cannot open input file");
    }

    std::streampos file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> buffer(file_size);
    file.read(reinterpret_cast<char*>(buffer.data()), file_size);
    file.close();

    auto iend_pos = std::search(buffer.begin(), buffer.end(), iend_chunk.begin(), iend_chunk.end());

    if (iend_pos == buffer.end()) {
        throw std::runtime_error("IEND chunk not found");
    }

    iend_pos += iend_chunk.size() + 4; // Move to the position after IEND chunk and its CRC

    buffer.erase(iend_pos, buffer.end());

    std::ofstream output(filename, std::ios::binary | std::ios::trunc);
    if (!output) {
        throw std::runtime_error("Cannot open output file");
    }

    output.write(reinterpret_cast<char*>(buffer.data()), buffer.size());
    output.close();
}

void write_size_metadata(const char* filepath, int a, int b, int c)
{
    std::ostringstream oss;
    oss << a << 'x' << b << 'x' << c;
    std::string text = oss.str();
    try {
        remove_custom_metadata(filepath);
        append_text_to_file(filepath, text);

    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        exit(1);
    }
}