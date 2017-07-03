#include "../Polynomial.h"
#include "../catamorph/Factories.h"
#include "../catamorph/interpreters/Dependencies.h"
#include <chrono>
#include <fstream>
#include <gperftools/profiler.h>
#include <iostream>

/*
int main() {
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    ProfilerStart("MyProfileFilename");

    Expression e = Factories::add(
        {Factories::cst(1), Factories::cst(2),
         Factories::mul({Factories::cst(0), Factories::var((ID) "x"),
                         Factories::var((ID) "y")}),
         Factories::mul(
             {Factories::cst(1), Factories::var((ID) "y"), Factories::cst(2)}),

         Factories::add({Factories::cst(0), Factories::var((ID) "x")})});
    Polynomial p = Polynomial(e);
    p.print();

    Env env = {{"x", 0}, {"y", 0}};
    for (int i = 0; i < 1000; i++) {
        for (int j = 0; j < 1000; j++) {
            p = Polynomial(e);
            env = {{"x", i}, {"y", j}};
            p.evaluate(env);
        }
    }

    ProfilerStop();

    end = std::chrono::system_clock::now();

    int elapsed_seconds =
        std::chrono::duration_cast<std::chrono::seconds>(end - start).count();

    std::cout
        << "1.000.000 evaluations on the expression (+ 1.000000 2.000000 "
           "(* 0.000000 x y) (* 1.000000 y 2.000000) (+ 0.000000 x)) required: "
        << elapsed_seconds << " seconds " << std::endl;
    return 0;
}
*/

void run_benchmark_from_file_single_evlauation(int domain_size,
                                               std::string benchmark_file,
                                               std::string result_file) {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(1, domain_size);

    std::chrono::time_point<std::chrono::system_clock> start, end;

    std::ofstream r_file(result_file);

    if (r_file.is_open()) {
        r_file << "File, Line, Var Count ,Seconds" << std::endl;
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
                           << variables.size() << "," << elapsed_seconds
                           << std::endl;
                    r_file.flush();
                    l_count++;
                } catch (...) {
                    r_file << benchmark_file << "," << l_count << ","
                           << variables.size() << ", ERROR " << std::endl;
                    r_file.flush();
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
        "benchmarkfiles/max_level_3",  "benchmarkfiles/max_level_4",
        "benchmarkfiles/max_level_6",  "benchmarkfiles/max_level_8",
        "benchmarkfiles/max_level_10", "benchmarkfiles/max_level_12",
        "benchmarkfiles/max_level_14", "benchmarkfiles/max_level_16",
        "benchmarkfiles/max_level_18", "benchmarkfiles/max_level_20",
        "benchmarkfiles/max_level_22", "benchmarkfiles/max_level_24"};

    for (std::string benchmark_file : benchmark_files) {
        run_benchmark_from_file_single_evlauation(
            default_domain_size, benchmark_file,
            benchmark_file + "_result.csv");
    }

    return 0;
}
