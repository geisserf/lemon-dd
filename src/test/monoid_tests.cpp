#include "Catch/include/catch.hpp"

#include "../evmdd/monoid.h"

using std::vector;

TEST_CASE("Neutral elements for monoids", "[monoids]") {
    SECTION("Neutral element for numbers is zero") {
        REQUIRE(Monoid<int>::neutral_element().get_value() == 0);
        REQUIRE(Monoid<double>::neutral_element().get_value() == 0);
        REQUIRE(Monoid<float>::neutral_element().get_value() == 0);
    }

    SECTION("Neutral element for variable assignments is the empty set") {
        REQUIRE(Monoid<Facts>::neutral_element().get_value().empty());
    }
}

SCENARIO("String printing of monoids", "[monoids]") {
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
        Monoid<Facts> fact_monoid(facts);
        REQUIRE(fact_monoid.to_string() == "{a=1 b=2 c=3}");
    }
}

SCENARIO("Greatest lower bound of monoids", "[monoids]") {
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
        Monoid<Facts> fact_monoid1(facts1);

        Facts facts2{{"a", 1}, {"b", 2}};
        Monoid<Facts> fact_monoid2(facts2);

        Facts facts3{{"a", 1}, {"c", 3}};
        Monoid<Facts> fact_monoid3(facts3);

        Facts facts4{{"a", 2}, {"c", 2}};
        Monoid<Facts> fact_monoid4(facts4);

        vector<Monoid<Facts>> all{fact_monoid1, fact_monoid2, fact_monoid3};
        vector<Monoid<Facts>> ab_and_abc{facts1, facts2};
        vector<Monoid<Facts>> ac_and_abc{facts1, facts3};
        vector<Monoid<Facts>> empty_intersection{facts3, facts4};

        REQUIRE(Monoid<Facts>::greatest_lower_bound(all).to_string() ==
                "{a=1}");
        REQUIRE(Monoid<Facts>::greatest_lower_bound(ab_and_abc).to_string() ==
                "{a=1 b=2}");
        REQUIRE(Monoid<Facts>::greatest_lower_bound(ac_and_abc).to_string() ==
                "{a=1 c=3}");
        REQUIRE(Monoid<Facts>::greatest_lower_bound(empty_intersection) ==
                Monoid<Facts>::neutral_element());
    }
}
