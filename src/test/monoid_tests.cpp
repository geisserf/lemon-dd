#include "Catch/include/catch.hpp"

#include "../evmdd/monoid.h"

using std::vector;

using FactMonoid = Monoid<Facts, Union>;
using IntFactMonoid = ProductMonoid<int, Facts, std::plus<int>, Union>;

TEST_CASE("Operator+ of monoids", "[monoids]") {
    SECTION("Operator+ for number monoids") {
        Monoid<int> one(1);
        Monoid<int> two(2);
        auto three = one + two;
        REQUIRE(three.get_value() == 3);
    }

    SECTION("Operator+ for fact monoids") {
        FactMonoid monoid_a(Facts{{"a", 1}});
        FactMonoid monoid_b(Facts{{"b", 2}});
        FactMonoid addition = monoid_a + monoid_b + monoid_a;
        REQUIRE(addition.to_string() == "{a=1 b=2}");
    }

    SECTION("Operator+ for product monoids") {
        IntFactMonoid first{1, Facts{{"a", 1}}};
        IntFactMonoid second{2, Facts{{"b", 2}}};
        auto addition = first + second;
        REQUIRE(addition.to_string() == "3,{a=1 b=2}");
    }
}

TEST_CASE("Operator- of monoids", "[monoids]") {
    SECTION("Operator- for number monoids") {
        Monoid<int> one(1);
        Monoid<int> two(2);
        auto diff = two - one;
        REQUIRE(diff.get_value() == 1);
    }

    SECTION("Operator- for fact monoids") {
        FactMonoid monoid_ab(Facts{{"a", 1},{"b",2}});
        FactMonoid monoid_b(Facts{{"b", 2}});
        FactMonoid diff = monoid_ab - monoid_b;
        REQUIRE(diff.to_string() == "{a=1}");
    }

    SECTION("Operator- for product monoids") {
        IntFactMonoid first{1, Facts{{"a", 1}}};
        IntFactMonoid second{2, Facts{{"b", 2}}};
        auto diff = second - first;
        REQUIRE(diff.to_string() == "1,{b=2}");
    }
}

TEST_CASE("Neutral elements for monoids", "[monoids]") {
    SECTION("Neutral element for numbers is zero") {
        REQUIRE(Monoid<int>::neutral_element() == 0);
        REQUIRE(Monoid<double>::neutral_element() == 0);
        REQUIRE(Monoid<float>::neutral_element() == 0);
    }

    SECTION("Neutral element for variable assignments is the empty set") {
        auto neutral = FactMonoid::neutral_element();
        REQUIRE(neutral.empty());
    }

    SECTION("Neutral element for product of numbers and variable assignments") {
        auto neutral = IntFactMonoid::neutral_element();
        REQUIRE(neutral.first == 0);
        REQUIRE(neutral.second.empty());
    }
}

TEST_CASE("String printing of monoids", "[monoids]") {
    SECTION("String printing for number monoids") {
        Monoid<int> i(1);
        REQUIRE(i.to_string() == "1");

        Monoid<double> d(5.5);
        REQUIRE(d.to_string() == "5.5");

        Monoid<float> f(2.5);
        REQUIRE(f.to_string() == "2.5");
    }

    SECTION("String printing for variable assignment monoids") {
        Facts facts{{"a", 1}, {"b", 2}, {"c", 3}};
        FactMonoid fact_monoid(facts);
        REQUIRE(fact_monoid.to_string() == "{a=1 b=2 c=3}");
    }

    SECTION("String printing for product monoid") {
        Facts facts{{"a", 5}};
        IntFactMonoid product{1, facts};
        REQUIRE(product.to_string() == "1,{a=5}");
    }
}

TEST_CASE("Greatest lower bound of monoids", "[monoids]") {
    SECTION("Greatest lower bound for number monoids") {
        REQUIRE(Monoid<int>::greatest_lower_bound(1, 3) == 1);
        REQUIRE(Monoid<float>::greatest_lower_bound(1.5, 5) == 1.5);
        REQUIRE(Monoid<double>::greatest_lower_bound(1.5, 2.2) == 1.5);
    }

    SECTION("Greatest lower bound for variable assignment monoids") {
        Facts abc{{"a", 1}, {"b", 2}, {"c", 3}};
        Facts ab{{"a", 1}, {"b", 2}};
        Facts ac{{"a", 1}, {"c", 3}};
        Facts other_ac{{"a", 2}, {"c", 2}};

        Facts glb = FactMonoid::greatest_lower_bound(ab, abc);
        std::string res = Monoid<Facts, Union>{glb}.to_string();
        REQUIRE("{a=1 b=2}" == res);
        glb = FactMonoid::greatest_lower_bound(ac, abc);
        res = Monoid<Facts, Union>{glb}.to_string();
        REQUIRE("{a=1 c=3}" == res);
        REQUIRE(FactMonoid::greatest_lower_bound(ac, other_ac) ==
                FactMonoid::neutral_element());
    }

    SECTION("Greatest lower bound for product monoid") {
        Fact a{"a", 1};
        Fact b{"b", 1};
        Fact c{"c", 1};
        std::pair<int, Facts> product_one_ab{1, Facts{a, b}};
        std::pair<int, Facts> product_two_ac(2, Facts{a, c});
        auto glb =
            IntFactMonoid::greatest_lower_bound(product_one_ab, product_two_ac);
        REQUIRE(IntFactMonoid{glb}.to_string() == "1,{a=1}");
        std::pair<int, Facts> product_one_a{1, Facts{a}};
        std::pair<int, Facts> product_two_b{2, Facts{b}};
        glb = IntFactMonoid::greatest_lower_bound(product_one_a, product_two_b);
        REQUIRE(IntFactMonoid{glb}.to_string() == "1,{}");
    }
}
