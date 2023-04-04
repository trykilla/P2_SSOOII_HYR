#include <iostream>
#include <thread>
#include <string>
#include <my_lib.h>
#include <bits/stdc++.h>
#include <thread_struct.h>
#include <mutex>
#include <shared_mutex>
#include <chrono>
#include <signal.h>

void find_word_in_file(std::string file_name, int i);
void print_results(int threads_num, std::string word);
void signal_handler(int signum);

std::shared_mutex mtx;

std::vector<thread_struct> v_thread_struct;

int main(int argc, char const *argv[])
{
    signal(SIGINT, signal_handler);
    parse_args(argc);

    std::vector<std::thread> v_threads;

    std::string file_name = argv[1];
    std::string word = argv[2];
    int threads_num = atoi(argv[3]);

    int lines_num = count_lines_in_file(file_name);

    int lines_per_thread = lines_num / threads_num;

    int id = 0;
    int init_line = 0;
    int final_line = 0;

    for (int i = 0; i < threads_num; i++)
    {
        id = i;
        init_line = i * lines_per_thread;
        final_line = init_line + lines_per_thread - 1;
        if (i == threads_num - 1)
        {
            final_line = lines_num - 1;
        }

        v_thread_struct.push_back(thread_struct{id, init_line, final_line, word, std::queue<result_struct>()});

        v_threads.push_back(std::thread(find_word_in_file, file_name, i));
    }

    std::for_each(v_threads.begin(), v_threads.end(), [](std::thread &t)
                  { t.join(); });

    print_results(threads_num, word);

    return EXIT_SUCCESS;
}

void signal_handler(int signum)
{
    std::cout << "\n[!] Interrupt signal (" << signum << ") received." << std::endl;
    exit(signum);
}

void print_results(int threads_num, std::string word)
{
    for (int i = 0; i < threads_num; i++)
    {

        while (!v_thread_struct[i].result.empty())
        {
            std::cout << "[Hilo " << i << " inicio:" << v_thread_struct[i].initial_line << " - final: " << v_thread_struct[i].final_line << "]";
            std::cout << " :: línea " << v_thread_struct[i].result.front().line << " :: ... " << v_thread_struct[i].result.front().pre_word << " "
                      << "\x1B[3m" << word << "\x1B[0m"
                      << " " << v_thread_struct[i].result.front().post_word << " ..." << std::endl;
            v_thread_struct[i].result.pop();
        }
    }
}

void find_word_in_file(std::string file_name, int thread_v_pos)
{

    int current_line = 0;
    std::fstream file;
    std::string line;

    file.open(file_name.c_str());

    std::string current_word, previous_word;

    my_to_upper(v_thread_struct[thread_v_pos].word);

    while (getline(file, line) && current_line <= v_thread_struct[thread_v_pos].final_line)
    {
        result_struct result_str;
        if (current_line >= v_thread_struct[thread_v_pos].initial_line)
        {
            std::istringstream ss(line);
            std::string previous_word, current_word, next_word;
            while (ss >> current_word)
            {
                my_to_upper(current_word);
                if (test_each_symbol(v_thread_struct[thread_v_pos].word, current_word))
                {

                    my_to_lower(previous_word);
                    result_str.pre_word = previous_word;
                    result_str.post_word = next_word;
                    result_str.line = current_line + 1;

                    if (ss >> next_word)
                    {

                        result_str.post_word = next_word;
                    }
                    else
                    {
                        result_str.post_word = "[No hay palabra posterior]";
                    }
                    std::unique_lock<std::shared_mutex> lock(mtx);
                    v_thread_struct[thread_v_pos].result.push(result_str);
                    lock.unlock();
                }
                previous_word = current_word;
            }
        }
        current_line++;
    }
    file.close();
}
