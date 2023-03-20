#include <iostream>
#include <thread>
#include <string>
#include <my_lib.h>
#include <bits/stdc++.h>

void find_word_in_file(std::string file_name);
void count_lines_in_file(std::string file_name);

int main(int argc, char const *argv[])
{

    parse_args(argc);
    std::string file_name = argv[1];
    std::thread main_thread(count_lines_in_file, file_name);

    main_thread.join();

    return EXIT_SUCCESS;
}

void find_word_in_file(std::string file_name)
{
    std::fstream file;
    std::string word;
    file.open(file_name.c_str());

    while (file >> word)
    {
        std::cout << word << std::endl;
    }
}

void count_lines_in_file(std::string file_name)
{
    std::string line;
    std::ifstream file(file_name);
    int lines_num = 0;

    if (file.is_open())
    {
        while (!file.eof())
        {
            getline(file, line);
            std::cout << line << std::endl;
            lines_num++;
        }
        file.close();
    }
    std::cout << lines_num << std::endl;
}
