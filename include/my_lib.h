#include <iostream>
#include <thread>
#include <bits/stdc++.h>
#include <string>
#include <fstream>
#include <vector>
#include <ctype.h>

int count_lines_in_file(std::string file_name);
void find_word_in_file(std::string file_name);
void my_to_upper(std::string &word);
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

void find_word_in_file(std::string file_name, std::string word)
{
    std::fstream file;
    file.open(file_name.c_str());

    /*quiero que con la palabra que te paso la busques en el archivo y me imprimas la palabra anterior y la posterior*/
    std::string current_word, previous_word;
    std::vector<std::string> matches;

    my_to_upper(word);

    std::string line;

    while (getline(file, line))
    {
        std::istringstream ss(line);
        std::string previous_word, current_word, next_word;
        while (ss >> current_word)
        {
            my_to_upper(current_word);
            if (test_each_symbol(word, current_word))
            {
                std::cout << "\nPrevious word: " << previous_word << "; current word: " << current_word << "; next word: ";
                if (ss >> next_word)
                {
                    std::cout << next_word << std::endl;
                }
            }
            previous_word = current_word;
        }
    }
    file.close();
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
