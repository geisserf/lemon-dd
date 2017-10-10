#include "../catamorph/interpreters/dependencies.h"
#include "../evmdd/evmdd.h"
#include "../evmdd/printer.h"
#include "../polynomial.h"
#include "../utils/system_utils.h"

#include <chrono>
#include <experimental/filesystem>
#include <fstream>
#include <gperftools/profiler.h>
#include <iostream>

using Time = std::chrono::steady_clock;
using ms = std::chrono::milliseconds;
using util = SystemUtils;

using std::chrono::time_point;
using std::string;
using std::cout;
using std::endl;

namespace fs = std::experimental::filesystem;

template <typename M, typename F>
void create_dot(std::ostream &output_stream, Evmdd<M, F> const &evmdd,
                Ordering const &o) {
    DotPrinter<M, F> printer(o);
    printer.to_dot(output_stream, evmdd);
}

void execute_benchmark(std::ostream &output_stream, string const &expression,
                       std::ostream &dot_stream) {
    time_point<Time> start, end;
    Polynomial p = Polynomial(expression);

    std::set<ID> variables = Dependency::dependencies(p.get_expression());
    Domains d;
    Ordering o;

    int i = 1;
    for (ID id : variables) {
        o.insert({id, i});
        d.insert({id, 2});
        i++;
    }

    cout << "Current RAM usage (KB): " << util::get_ram_used_by_this() << endl;
    start = Time::now();
    ProfilerStart("evmdd.profile");
    auto evmdd = p.create_evmdd<double>(d, o);
    ProfilerStop();
    end = Time::now();

    int elapsed_time = std::chrono::duration_cast<ms>(end - start).count();
    int size = evmdd.size();
    output_stream << "size (nodes): " << std::to_string(size) << endl;
    output_stream << "time (ms): " << std::to_string(elapsed_time) << endl;
    output_stream << "memory(kb): " << util::get_ram_used_by_this() << endl;
    cout << "Size (nodes): " << std::to_string(size) << endl;
    cout << "Duration (ms): " << std::to_string(elapsed_time) << endl;
    cout << "Current RAM usage (KB): " << util::get_ram_used_by_this() << endl;
    create_dot(dot_stream, evmdd, o);
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
    string filename = result_dir + "/" + fs_path.stem().string();
    string result_file = filename + ".result";
    string dot_file = filename + ".dot";
    cout << "Executing " << filepath << endl;
    cout << "Saved results in " << result_file << endl;
    std::ofstream result_stream(result_file, std::ios_base::app);
    std::ofstream dot_stream(dot_file);
    execute_benchmark(result_stream, expression, dot_stream);
}
