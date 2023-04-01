#include <iostream>
#include <thread>
#include <string>
#include <my_lib.h>
#include <bits/stdc++.h>
#include <thread_struct.h>
#include <chrono>

int main(int argc, char const *argv[])
{
    int lines_num = 0;

    parse_args(argc);

    std::string file_name = argv[1];
    std::string word = argv[2];
    // int threads_num = 3;
    int threads_num = atoi(argv[3]);

    std::vector<std::thread> v_threads;
    std::vector<thread_struct> v_thread_struct;

    lines_num = count_lines_in_file(file_name);
    std::cout << "Lines num: " << lines_num << std::endl;

    int lines_per_thread = lines_num / threads_num;

    for (int i = 0; i < threads_num; i++)
    {
        // 12 hilos
        int id = i;
        int init_line = i * lines_per_thread;
        int final_line = init_line + lines_per_thread - 1;
        std::cout << "Thread " << i << " init_line: " << init_line << " final_line: " << final_line << std::endl;
        if (i == threads_num - 1)
        {
            final_line = lines_num - 1;
        }

        v_thread_struct.push_back(thread_struct{id, init_line, final_line, word, std::queue<result_struct>()});
        v_threads.push_back(std::thread(find_word_in_file, file_name, std::ref(v_thread_struct[i])));
        std::cout << "Thread " << i << " created" << std::endl;
        sleep(1);
    }

    std::for_each(v_threads.begin(), v_threads.end(), [](std::thread &t)
                  { t.join(); });

    for (int i = 0; i < threads_num; i++)
    {
        std::cout << "Thread " << i << " results: " << std::endl;
        while (!v_thread_struct[i].result.empty())
        {
            std::cout << "Line: " << v_thread_struct[i].result.front().line << " Pre_word: " << v_thread_struct[i].result.front().pre_word << " Post_word: " << v_thread_struct[i].result.front().post_word << std::endl;
            v_thread_struct[i].result.pop();
        }
    }

    return EXIT_SUCCESS;
}
