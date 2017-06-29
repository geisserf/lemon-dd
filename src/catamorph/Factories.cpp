//
// Created by bwright on 22.06.17.
//

#include "Factories.h"

Expression Factories::mul(std::vector<Expression> const &rands) {
    return Expression(mul_op<Expression>{rands});
}

Expression Factories::sub(std::vector<Expression> const &rands) {
    return Expression(sub_op<Expression>{rands});
}

Expression Factories::add(std::vector<Expression> const &rands) {
    return Expression(add_op<Expression>{rands});
}

Expression Factories::var(ID id) {
    return Expression(id);
}

Expression Factories::cst(float i) {
    return Expression(i);
}

Expression Factories::div(std::vector<Expression> const &rands) {
    return Expression(div_op<Expression>{rands});
}

Expression Factories::land(std::vector<Expression> const &rands) {
    return Expression(and_op<Expression>{rands});
}

Expression Factories::equals(std::vector<Expression> const &rands) {
    return Expression(equals_op<Expression>{rands});
}
