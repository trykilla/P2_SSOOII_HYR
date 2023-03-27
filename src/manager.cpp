#include <iostream>
#include <thread>
#include <string>
#include <my_lib.h>
#include <bits/stdc++.h>



int main(int argc, char const *argv[])
{
    int lines_num = 0;
    parse_args(argc);
    std::string file_name = argv[1];
    std::string word = argv[2];
    int threads_num = atoi(argv[3]);

    lines_num = count_lines_in_file(file_name);
    std::cout << "Lines num: " << lines_num << std::endl;

    find_word_in_file(file_name, word);

    return EXIT_SUCCESS;
}
