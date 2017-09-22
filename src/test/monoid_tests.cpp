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
        IntFactMonoid first{1, Facts{{"a",1}}};
        IntFactMonoid second{2, Facts{{"b",2}}};
        auto addition = first + second;
        REQUIRE(addition.to_string() == "3,{a=1 b=2}");
    }
}

TEST_CASE("Neutral elements for monoids", "[monoids]") {
    SECTION("Neutral element for numbers is zero") {
        REQUIRE(Monoid<int>::neutral_element().get_value() == 0);
        REQUIRE(Monoid<double>::neutral_element().get_value() == 0);
        REQUIRE(Monoid<float>::neutral_element().get_value() == 0);
    }

    SECTION("Neutral element for variable assignments is the empty set") {
        auto neutral = FactMonoid::neutral_element();
        REQUIRE(neutral.get_value().empty());
    }

    SECTION("Neutral element for product of numbers and variable assignments") {
        auto neutral = IntFactMonoid::neutral_element();
        REQUIRE(neutral.first().get_value() == 0);
        REQUIRE(neutral.second().get_value().empty());
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
        Monoid<int> i1(1);
        Monoid<int> i2(3);
        Monoid<int> i3(5);
        vector<Monoid<int>> int_vector{i1, i2, i3};
        REQUIRE(Monoid<int>::greatest_lower_bound(int_vector).get_value() == 1);

        Monoid<float> f1(1.5);
        Monoid<float> f2(3.3);
        Monoid<float> f3(5.6);
        vector<Monoid<float>> float_vector{f1, f2, f3};
        REQUIRE(Monoid<float>::greatest_lower_bound(float_vector).get_value() ==
                1.5);

        Monoid<double> d1(1.1);
        Monoid<double> d2(3.2);
        Monoid<double> d3(1.1);
        vector<Monoid<double>> double_vector{d1, d2, d3};
        REQUIRE(
            Monoid<double>::greatest_lower_bound(double_vector).get_value() ==
            1.1);
    }

    SECTION("Greatest lower bound for variable assignment monoids") {
        Facts facts1{{"a", 1}, {"b", 2}, {"c", 3}};
        FactMonoid fact_monoid1(facts1);

        Facts facts2{{"a", 1}, {"b", 2}};
        FactMonoid fact_monoid2(facts2);

        Facts facts3{{"a", 1}, {"c", 3}};
        FactMonoid fact_monoid3(facts3);

        Facts facts4{{"a", 2}, {"c", 2}};
        FactMonoid fact_monoid4(facts4);

        vector<FactMonoid> all{fact_monoid1, fact_monoid2, fact_monoid3};
        vector<FactMonoid> ab_and_abc{facts1, facts2};
        vector<FactMonoid> ac_and_abc{facts1, facts3};
        vector<FactMonoid> empty_intersection{facts3, facts4};

        REQUIRE("{a=1}" == FactMonoid::greatest_lower_bound(all).to_string());
        REQUIRE("{a=1 b=2}" ==
                FactMonoid::greatest_lower_bound(ab_and_abc).to_string());
        REQUIRE("{a=1 c=3}" ==
                FactMonoid::greatest_lower_bound(ac_and_abc).to_string());
        REQUIRE(FactMonoid::greatest_lower_bound(empty_intersection) ==
                FactMonoid::neutral_element());
    }

    SECTION("Greatest lower bound for product monoid") {
        Fact a{"a", 1};
        Fact b{"b", 1};
        Fact c{"c", 1};
        IntFactMonoid product_one_ab{1, Facts{a, b}};
        IntFactMonoid product_two_ac(2, Facts{a, c});
        auto glb = IntFactMonoid::greatest_lower_bound(
            {product_one_ab, product_two_ac});
        REQUIRE(glb.to_string() == "1,{a=1}");
        IntFactMonoid product_one_a{1, Facts{a}};
        IntFactMonoid product_two_b{2, Facts{b}};
        glb =
            IntFactMonoid::greatest_lower_bound({product_one_a, product_two_b});
        REQUIRE(glb.to_string() == "1,{}");
    }
}
