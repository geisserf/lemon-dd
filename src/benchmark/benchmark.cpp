#include "../Polynomial.h"
#include "../catamorph/Factories.h"
#include "../catamorph/interpreters/Dependencies.h"
#include <chrono>
#include <fstream>
#include <gperftools/profiler.h>
#include <iostream>

void run_benchmark_from_file_single_evlauation(int domain_size,
                                               std::string benchmark_file,
                                               std::string result_file,
                                               int ast_depth) {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(1, domain_size);

    std::chrono::time_point<std::chrono::system_clock> start, end;

    std::ofstream r_file(result_file, std::ios_base::app);

    if (r_file.is_open()) {
        std::ifstream b_file(benchmark_file);
        if (b_file.is_open()) {
            std::string line;
            int l_count = 1;
            while (std::getline(b_file, line)) {
                start = std::chrono::system_clock::now();
                Polynomial p = Polynomial(line);
                std::set<ID> variables =
                    Dependency::dependencies(p.getExpression());
                Env env;
                for (ID id : variables) {
                    env.insert(std::pair<std::string, int>(id, dist(mt)));
                }

                try {
                    p.evaluate(env);
                    end = std::chrono::system_clock::now();
                    int elapsed_seconds =
                        std::chrono::duration_cast<std::chrono::milliseconds>(
                            end - start)
                            .count();

                    r_file << benchmark_file << "," << l_count << ","
                           << variables.size() << "," << elapsed_seconds << ","
                           << ast_depth << std::endl;
                    r_file.flush();
                    l_count++;
                } catch (...) {
                }
            }
        } else {
            std::cout << "unable to open benchmark file" << std::endl;
        }
    } else {
        std::cout << "unable to open result file" << std::endl;
    }
}

int main() {
    int default_domain_size = 1000;

    std::array<std::string, 13> benchmark_files = {
        "benchmarkfiles/max_level_2",  "benchmarkfiles/max_level_4",
        "benchmarkfiles/max_level_6",  "benchmarkfiles/max_level_8",
        "benchmarkfiles/max_level_10", "benchmarkfiles/max_level_12",
        "benchmarkfiles/max_level_14"};

    std::string result_file = "benchmarkfiles/catamorph_results";
    std::ofstream r_file(result_file);
    if (r_file.is_open()) {
        r_file << "File, Line, Var Count ,Seconds, AstDepth" << std::endl;
    }
    int depth = 2;
    for (std::string benchmark_file : benchmark_files) {
        run_benchmark_from_file_single_evlauation(
            default_domain_size, benchmark_file, result_file, depth);
        depth = depth + 2;
    }

    return 0;
}
