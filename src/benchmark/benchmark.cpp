#include "../Polynomial.h"
#include "../catamorph/Factories.h"
#include <gperftools/profiler.h>
#include <unistd.h>

int main() {
    ProfilerStart("MyProfileFilename");

    Expression e = Factories::add(
        {Factories::cst(1), Factories::cst(2),
         Factories::mul({Factories::cst(0), Factories::var((ID) "x"),
                         Factories::var((ID) "y")}),
         Factories::mul(
             {Factories::cst(1), Factories::var((ID) "y"), Factories::cst(2)}),






         Factories::add({Factories::cst(0), Factories::var((ID) "x")})});

    Env env = {{"x", 0}, {"y", 0}};
    for (int i = 0; i < 1000; i++) {
        for (int j = 0; j < 1000; j++) {
            Polynomial p = Polynomial(e);
            env = {{"x", i}, {"y", j}};
            p.evaluate(env);
            p.print();
        }
    }

    ProfilerStop();
    return 0;
}