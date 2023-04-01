#include <iostream>
#include <thread>
#include <bits/stdc++.h>
#include <string>
#include <fstream>
#include <vector>
#include <ctype.h>
#include <thread_struct.h>

int count_lines_in_file(std::string file_name);
void find_word_in_file(std::string file_name, thread_struct &thread_str);
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

void find_word_in_file(std::string file_name, thread_struct &thread_str)
{
    int current_line = 0;
    std::fstream file;
    file.open(file_name.c_str());

    /*quiero que con la palabra que te paso la busques en el archivo y me imprimas la palabra anterior y la posterior*/
    std::string current_word, previous_word;
    std::vector<std::string> matches;

    my_to_upper(thread_str.word);

    std::string line;

    while (getline(file, line) && current_line <= thread_str.final_line)
    {
        result_struct result_str;
        if (current_line >= thread_str.initial_line)
        {
            std::istringstream ss(line);
            std::string previous_word, current_word, next_word;
            while (ss >> current_word)
            {
                my_to_upper(current_word);
                if (test_each_symbol(thread_str.word, current_word))
                {
                    result_str.pre_word = previous_word;
                    result_str.post_word = next_word;
                    result_str.line = current_line + 1;

                    if (ss >> next_word)
                    {

                        result_str.post_word = next_word;
                    }
                    else
                    {
                        result_str.post_word = "[No next word in current line]";
                    }
                    thread_str.result.push(result_str);
                }
                previous_word = current_word;
            }
        }
        current_line++;
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
