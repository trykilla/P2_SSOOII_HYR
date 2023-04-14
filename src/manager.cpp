/************************************************************************************
 * Proyecto: Práctica 2 - Sistemas Operativos II                                    *
 * Nombre del programa: manager.cpp                                                 *                                                       
 * Autores: Héctor Alberca Sánchez-Quintanar y Rubén Crespo Calcerrada              *
 * Fecha: 8/04/2023                                                                *
 * Propósito: Programa que gestiona los hilos                                       *   
 ************************************************************************************/


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

/* Se definen las funciones que se van a utilizar */
void find_word_in_file(std::string file_name, int i);
void print_results(int threads_num, std::string word);
void signal_handler(int signum);

/*Se define tanto el semáforo compartido como el vector de estructuras de hilos*/

std::shared_mutex mtx;

std::vector<thread_struct> v_thread_struct;

int main(int argc, char const *argv[])
{
    /*  Se define el manejador de señal en caso de que el usuario quiera parar el programa y
         se comprueban que los argumentos son los esperados   */

    signal(SIGINT, signal_handler);
    parse_args(argc);

    /*  Se definen las variables nesarias para el programa y además se atribuyen una variable
        a cada uno de los argumentos introducidos.     */

    std::vector<std::thread> v_threads;

    std::string file_name = argv[1];
    std::string word = argv[2];
    int threads_num = atoi(argv[3]);

    int lines_num = count_lines_in_file(file_name);

    int lines_per_thread = lines_num / threads_num;

    int id = 0;
    int init_line = 0;
    int final_line = 0;

    /*  Se crean los hilos y se les asigna una estructura de hilo con los datos necesarios para
        su ejecución.   */

    for (int i = 0; i < threads_num; i++)
    {
        //Se dividen las líneas entre los hilos
        id = i;
        init_line = i * lines_per_thread;
        final_line = init_line + lines_per_thread - 1;
        if (i == threads_num - 1)
        {
            final_line = lines_num - 1;
        }

        v_thread_struct.push_back(thread_struct{id, init_line, final_line, word, std::queue<result_struct>()});

        //Se crean los hilos y se añaden en el vector de hilos, a su vez se llama a la función find_word_in_file, 
        //que es la que se encarga de buscar la palabra en el fichero.
        v_threads.push_back(std::thread(find_word_in_file, file_name, i));
    }

    //Se espera a que todos los hilos terminen su ejecución y se imprime el resultado de cada uno de ellos.
    std::for_each(v_threads.begin(), v_threads.end(), [](std::thread &t)
                  { t.join(); });

    print_results(threads_num, word);

    return EXIT_SUCCESS;
}

/**
 * Maneja las señales de interrupción del programa.
 *
 * @param signum Número de la señal
 *
 * @returns No devuelve nada
 */

void signal_handler(int signum)
{
    std::cout << "\n[!] Interrupt signal (" << signum << ") received." << std::endl;
    exit(signum);
}

/**
 * Imprime los resultados de la búsqueda de la palabra en el fichero.
 *
 * @param threads_num Número de hilos
 * @param word Palabra a buscar
 *
 * @returns No devuelve nada
 */

void print_results(int threads_num, std::string word)
{
    for (int i = 0; i < threads_num; i++)
    {
        //Se imprime el resultado de cada hilo hasta que el vector de resultados de cada hilo esté vacío.    
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

/**
 * Busca la palabra en el fichero.
 *
 * @param file_name Nombre del fichero
 * @param thread_v_pos Posición del vector de hilos
 *
 * @returns No devuelve nada
 */

void find_word_in_file(std::string file_name, int thread_v_pos)
{
    /*  Se inician y se crean las variables necesarias para la función. */
    int current_line = 0;
    std::fstream file;
    std::string line;

    /*  Se abre el fichero y se lee línea a línea.   */
    file.open(file_name.c_str());

    std::string current_word, previous_word;

    //Se convierte la palabra a mayúsculas para que no haya problemas con las mayúsculas y minúsculas.
    my_to_upper(v_thread_struct[thread_v_pos].word);

    /* Se lee el fichero línea a línea hasta que se llega a la línea final del hilo que está realizando en este momento. */
    while (getline(file, line) && current_line <= v_thread_struct[thread_v_pos].final_line)
    {
        result_struct result_str;
        if (current_line >= v_thread_struct[thread_v_pos].initial_line)
        {
            /*  Cogemos una línea y la dividimos en las palabras que tiene, vamos comprobando palabra por palabra 
                si es la que estamos buscando. En caso de que sea esa, en la estructura result_struct guardamos 
                la palabra anterior, la posterior y la línea donde la hemos encontrado.   */

            std::istringstream words_in_line(line);
            std::string previous_word, current_word, next_word;
            while (words_in_line >> current_word)
            {
                my_to_upper(current_word);
                if (test_each_symbol(v_thread_struct[thread_v_pos].word, current_word))
                {

                    my_to_lower(previous_word);
                    result_str.pre_word = previous_word;
                    result_str.line = current_line + 1;

                    if (words_in_line >> next_word)
                    {

                        result_str.post_word = next_word;
                    }
                    else
                    {
                        result_str.post_word = "[No hay palabra posterior]";
                    }
                    /*  Utilizamos el semáforo para añadir información en el vector de estructuras de hilos compartidos. 
                        De esta manera, no acceden varios semáforos al mismo tiempo al ser una sección crítica.*/
                    std::unique_lock<std::shared_mutex> lock(mtx);
                    v_thread_struct[thread_v_pos].result.push(result_str);
                    lock.unlock();
                }
                previous_word = current_word;
            }
        }
        current_line++;
    }
    //Se cierra el fichero.
    file.close();
}
