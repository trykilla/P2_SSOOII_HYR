#include <iostream>
#include <bits/stdc++.h>
#include <string>
#include <fstream>
#include <ctype.h>

int count_lines_in_file(std::string file_name);
void my_to_upper(std::string &word);
void my_to_lower(std::string &word);
bool test_each_symbol(std::string word, std::string current_word);

void parse_args(int argc)
{
    if (argc != 4)
    {
        std::cerr << "[!] Error in args line. Usage: ./main <file_name> <word> <threads_number> " << std::endl;
        exit(EXIT_FAILURE);
    }
}

int count_lines_in_file(std::string file_name)
{
    std::string line;
    std::ifstream file(file_name);
    int lines_num = 0;

    if (file.is_open())
    {
        while (!file.eof())
        {
            getline(file, line);
            lines_num++;
        }
        file.close();
    }
    return lines_num;
}

bool test_each_symbol(std::string word, std::string current_word)
{
    bool flag = false;
    if (current_word == word || current_word == word + "." || current_word == word + "," || current_word == word + ";")
    {
        flag = true;
    }
    return flag;
}

void my_to_upper(std::string &word)
{
    transform(word.begin(), word.end(), word.begin(), [](unsigned char c)
              { return toupper(c); });
}

void my_to_lower(std::string &word)
{
    transform(word.begin(), word.end(), word.begin(), [](unsigned char c)
              { return tolower(c); });
}