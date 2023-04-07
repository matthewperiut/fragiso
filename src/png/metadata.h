#pragma once

#include <string>

void append_text_to_file(const std::string& filename, const std::string& text);
std::string read_text_from_end_of_file(const std::string& filename);
void remove_custom_metadata(const std::string& filename);
void write_size_metadata(const char* filepath, int a, int b, int c);
