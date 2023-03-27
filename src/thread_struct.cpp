#include <iostream>
#include <thread>
#include <string>
#include <queue>

struct thread_struct
{
    int thread_id;
    int initial_line;
    int final_line;
    std::string word;
    std::queue <result_struct> result;
};

struct result_struct
{
    std::string pre_word;
    std::string post_word;
    int line;
};
