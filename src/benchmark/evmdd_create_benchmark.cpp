#include "../catamorph/interpreters/dependencies.h"
#include "../polynomial.h"

#include <chrono>
#include <experimental/filesystem>
#include <fstream>
#include <iostream>

using Time = std::chrono::steady_clock;
using ms = std::chrono::milliseconds;

using std::chrono::time_point;
using std::string;
using std::cout;
using std::endl;

namespace fs = std::experimental::filesystem;

void execute_benchmark(std::ostream &output_stream, string const &expression) {
    time_point<Time> start, end;
    Polynomial p = Polynomial(expression);

    std::set<ID> variables = Dependency::dependencies(p.getExpression());
    Domains d;
    Ordering o;

    int i = 1;
    for (ID id : variables) {
        o.insert({id, i});
        d.insert({id, 2});
        i++;
    }

    start = Time::now();
    p.create_evmdd(d, o);
    end = Time::now();

    int elapsed_time = std::chrono::duration_cast<ms>(end - start).count();
    output_stream << "time (ms): " << std::to_string(elapsed_time) << endl;
    cout << "Duration: " << std::to_string(elapsed_time) << endl;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        cout << "Usage: evmdd_create_benchmark <function_filename>" << endl;
        exit(0);
    }
    string filepath{argv[1]};

    if (!fs::exists(filepath)) {
        throw std::invalid_argument(filepath + " does not exist.");
    }

    string result_dir = "benchmarkfiles/results";

    try {
        fs::create_directory(result_dir);
    } catch (fs::filesystem_error err) {
        cout << "Could not create benchmark directory " << result_dir << endl;
        exit(0);
    }

    std::ifstream input_stream(filepath);
    string expression;
    std::getline(input_stream, expression);

    // Used to retrieve filename without path and filetype extension
    fs::path fs_path{filepath};

    string result_file = result_dir + "/" + fs_path.stem().string() + ".result";
    cout << "Executing " << filepath << endl;
    cout << "Results are saved in " << result_file << endl;
    std::ofstream result_stream(result_file, std::ios_base::app);
    execute_benchmark(result_stream, expression);
}
