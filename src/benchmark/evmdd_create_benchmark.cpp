#include "../catamorph/interpreters/dependencies.h"
#include "../polynomial.h"

#include <chrono>
#include <dirent.h>
#include <fstream>
#include <fstream>
#include <gperftools/profiler.h>
#include <iostream>
#include <sys/stat.h>

void execute_benchmark(std::string name, std::string expression) {
    std::cout << "Executing " << name << std::endl;

    // std::chrono::time_point<std::chrono::system_clock> start, end;

    Polynomial p = Polynomial(expression);
    /*
    std::set<ID> variables = Dependency::dependencies(p.getExpression());

    Domains d;
    Ordering o;

    int i = 1;
    for (ID id : variables) {
        o.insert({id, i});
        d.insert({id, 2});
        i++;
    }

    start = std::chrono::system_clock::now();
    p.create_evmdd(d, o);
    end = std::chrono::system_clock::now();

    int elapsed_seconds =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
            .count();
    std::cout << "Duration: " << std::to_string(elapsed_seconds) << std::endl;

    */

    std::cout << "Done" << std::endl;
}

int main() {
    DIR *dir;
    class dirent *ent;
    class stat st;

    std::string directory = "benchmarkfiles/Prost";

    dir = opendir(directory.c_str());

    if (dir) {
        while ((ent = readdir(dir)) != NULL) {
            const std::string file_name = ent->d_name;
            const std::string full_file_name = directory + "/" + file_name;

            if (file_name[0] == '.')
                continue;

            if (stat(full_file_name.c_str(), &st) == -1)
                continue;

            const bool is_directory = (st.st_mode & S_IFDIR) != 0;

            if (is_directory)
                continue;

            std::cout << "File: " << full_file_name << std::endl;

            std::ifstream benchmark_file(full_file_name);
            if (benchmark_file.is_open()) {
                std::string expression;
                std::string line;
                while (std::getline(benchmark_file, line)) {
                    expression = expression + " " + line;
                }
                execute_benchmark(file_name, expression);
            }
        }
    }

    closedir(dir);

    return 0;
}
