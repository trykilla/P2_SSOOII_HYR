#include <iostream>
#include <thread>
#include <string>
#include <my_lib.h>
#include <bits/stdc++.h>
#include <thread_struct.h>
#include <mutex>
#include <shared_mutex>
#include <chrono>

void find_word_in_file(std::string file_name, thread_struct &thread_str);

std::mutex mtx;

int main(int argc, char const *argv[])
{
    int lines_num = 0;

    parse_args(argc);

    std::string file_name = argv[1];
    std::string word = argv[2];
    int threads_num = atoi(argv[3]);

    std::vector<std::thread> v_threads;
    std::vector<thread_struct> v_thread_struct;

    lines_num = count_lines_in_file(file_name);
    std::cout << "Lines num: " << lines_num << std::endl;

    int lines_per_thread = lines_num / threads_num;

    for (int i = 0; i < threads_num; i++)
    {

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
    }

    std::for_each(v_threads.begin(), v_threads.end(), [](std::thread &t)
                  { t.join(); });

    for (int i = 0; i < threads_num; i++)
    {
        if (!v_thread_struct[i].result.empty())
        {
            while (!v_thread_struct[i].result.empty())
            {
                std::cout << "[Hilo " << i << " inicio:" << v_thread_struct[i].initial_line << " - final: " << v_thread_struct[i].final_line << "]";
                std::cout << " :: línea " << v_thread_struct[i].result.front().line << "] :: " << v_thread_struct[i].result.front().pre_word << " " << word << " " << v_thread_struct[i].result.front().post_word << std::endl;
                v_thread_struct[i].result.pop();
            }
        }
    }

    return EXIT_SUCCESS;
}

void find_word_in_file(std::string file_name, thread_struct &thread_str)
{

    int current_line = 0;
    std::fstream file;
    file.open(file_name.c_str());

    /*quiero que con la palabra que te paso la busques en el archivo y me imprimas la palabra anterior y la posterior*/
    std::string current_word, previous_word;

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

                    mtx.lock();
                    thread_str.result.push(result_str);
                    mtx.unlock();
                }
                previous_word = current_word;
            }
        }
        current_line++;
    }
}
