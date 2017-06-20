#include "catamorphism.h"
#include "eval.h"
#include "factories.h"
#include "printer.h"
#include <iostream>

using nb = float;
using id = std::string;

int main() {

  std::cout << "\033[1;33m 10-5\033[0m" << '\n';
  expression e1 = sub({cst(10), cst(5)});
  std::cout << eval({}, e1) << '\n';

  std::cout << "\033[1;33m 10+5\033[0m" << '\n';
  expression e2 = add({cst(10), cst(5)});

  std::cout << eval({}, e2) << '\n';

  std::cout << "\033[1;33m 10/2\033[0m" << '\n';
  expression e3 = div({cst(10), cst(2)});
  std::cout << eval({}, e3) << '\n';

  std::cout << "\033[1;33m 10*2\033[0m" << '\n';
  expression e4 = mul({cst(10), cst(2)});
  std::cout << eval({}, e4) << '\n';
  dependencies(e4);

  expression e5 =
      add({cst(1), cst(2), mul({cst(0), var((id) "x"), var((id) "y")}),
           mul({cst(1), var((id) "y"), cst(2)}), add({cst(0), var((id) "x")})});
  // Environment of evaluation
  env full_env = {{"x", 1}, {"y", 2}};
  env partial_env = {{"y", 2}};
  // Evaluations
  // std::cout << eval(full_env, e) << '\n';
  std::cout
      << "\033[1;33m (+ 1 2 (* 0 x y) (* 1 y 2) (+ 0 x)), x=1, y=2 \033[0m"
      << '\n';
  std::cout << eval(full_env, e5) << '\n';
  try {
    std::cout << "\033[1;33m (+ 1 2 (* 0 x y) (* 1 y 2) (+ 0 x)), y=2 \033[0m"
              << '\n';
    std::cout << eval(partial_env, e5) << '\n';
  } catch (std::logic_error e) {
    std::cout << "\033[1;31m Exception in eval: " << e.what() << "\033[0m"
              << std::endl;
  }

  std::cout << "\033[1;33m (+ 1 2 (* 0 x y) (* 1 y 2) (+ 0 x)), y=2 \033[0m"
            << '\n';
  auto o = partial_eval(partial_env, e5);
  std::cout << cata<std::string>(print_alg, o) << '\n';

  std::cout << "\033[1;33m 9/2\033[0m" << '\n';
  expression e6 = div({cst(9), cst(2)});
  std::cout << eval({}, e6) << '\n';

  std::cout << "\033[1;33m x==2\033[0m" << '\n';
  expression e7 = equals({var((id) "x"), cst(2)});
  o = partial_eval({}, e7);
  std::cout << cata<std::string>(print_alg, o) << '\n';

  std::cout << "\033[1;33m x==2, x=2\033[0m" << '\n';
  expression e8 = equals({var((id) "x"), cst(2)});
  o = partial_eval({{"x", 2}}, e8);
  std::cout << cata<std::string>(print_alg, o) << '\n';

  std::cout << "\033[1;33m x==2 ^ y==4\033[0m" << '\n';
  expression e9 =
      land({equals({var((id) "x"), cst(2)}), equals({var((id) "y"), cst(4)})});

  o = partial_eval({}, e9);
  std::cout << cata<std::string>(print_alg, o) << '\n';

  std::cout << "\033[1;33m x==2 ^ y==4, x=2,y=4\033[0m" << '\n';
  expression e10 =
      land({equals({var((id) "x"), cst(2)}), equals({var((id) "y"), cst(4)})});

  o = partial_eval({{"x", 2}, {"y", 4}}, e10);
  std::cout << cata<std::string>(print_alg, o) << '\n';

  return 0;
}