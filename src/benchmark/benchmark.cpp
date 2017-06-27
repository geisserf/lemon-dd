#include "../Polynomial.h"
#include "../catamorph/Factories.h"
#include <chrono>
#include <gperftools/profiler.h>
#include <iostream>

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