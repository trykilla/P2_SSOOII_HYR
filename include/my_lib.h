#include <iostream>
#include <thread>

void parse_args(int argc)
{
    if (argc != 4)
    {
        std::cerr << "[!] Error in args line. Usage: ./main <file_name> <word> <threads_number> " << std::endl;
    }
}