/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana.hpp>

#include <type_traits>


namespace sandbox {
constexpr struct sign_ {
    template <typename X>
    constexpr X operator()(X x) const {
        return x < X{0}  ? -X{1} :
               x == X{0} ?  X{0} :
                            X{1} ;
    }
} sign{};

constexpr struct abs_ {
    template <typename X>
    constexpr X operator()(X x) const {
        return x * sign(x);
    }
} abs{};

constexpr struct gcd_ {
    template <typename X>
    constexpr X operator()(X a, X b) const {
        return b == X{0} ? a : (*this)(b, a % b);
    }
} gcd{};

constexpr struct lcm_ {
    template <typename X>
    constexpr X operator()(X a, X b) const {
        return a == X{0} && b == X{0} ? X{0} : (a / gcd(a, b)) * b;
    }
} lcm{};

constexpr struct fac_ {
    template <typename X>
    constexpr X operator()(X n) const {
        return n == X{0} ? X{1} : n * (*this)(n - X{1});
    }
} fac{};

template <typename T>
struct ratio {
    T num, den;

    constexpr ratio(T numerator, T denominator = T{1})
        : num{sign(numerator) * sign(denominator) * abs(numerator) / gcd(numerator, denominator)},
          den{abs(denominator) / gcd(numerator, denominator)}
    { }

    template <typename U>
    constexpr ratio(ratio<U> other)
        : ratio{other.num, other.den}
    { }

    constexpr ratio operator+() const {
        return {num, den};
    }

    constexpr ratio operator-() const {
        return {-num, den};
    }

    template <typename Ostream>
    friend Ostream& operator<<(Ostream& os, ratio x) {
        if (x.den == 1) return os << x.num;
        else            return os << x.num << '/' << x.den;
    }
};

template <typename X, typename Y>
constexpr ratio<std::common_type_t<X, Y>> operator+(ratio<X> x, ratio<Y> y) {
    return {x.num * y.den + x.den * y.num, x.den * y.den};
}

template <typename X, typename Y>
constexpr auto operator-(ratio<X> x, ratio<Y> y) {
    return x + (-y);
}

template <typename X, typename Y>
constexpr ratio<std::common_type_t<X, Y>> operator*(ratio<X> x, ratio<Y> y) {
    return {x.num * y.num, x.den * y.den};
}

template <typename X, typename Y>
constexpr ratio<std::common_type_t<X, Y>> operator/(ratio<X> x, ratio<Y> y) {
    return {x.num * y.den, x.den * y.num};
}

template <typename X, typename Y>
constexpr bool operator==(ratio<X> x, ratio<Y> y) {
    return x.num * y.den == y.num * x.den;
}

template <typename X, typename Y>
constexpr bool operator!=(ratio<X> x, ratio<Y> y) {
    return !(x == y);
}


template <typename X, typename Y>
constexpr bool operator<(ratio<X> x, ratio<Y> y) {
    return x.num * y.den < y.num * x.den;
}

template <typename X, typename Y>
constexpr bool operator<=(ratio<X> x, ratio<Y> y) {
    return !(y < x);
}

template <typename X, typename Y>
constexpr bool operator>(ratio<X> x, ratio<Y> y) {
    return y < x;
}

template <typename X, typename Y>
constexpr bool operator>=(ratio<X> x, ratio<Y> y) {
    return !(x < y);
}


constexpr ratio<unsigned long long> operator"" _r(unsigned long long num) {
    return {num};
}

//////////////////////////////////////////////////////////////////////////////

static_assert(sign(-100) == -1, "");
static_assert(sign(0) == 0, "");
static_assert(sign(20) == 1, "");

static_assert(abs(0) == 0, "");
static_assert(abs(20) == 20, "");
static_assert(abs(-20) == 20, "");

static_assert(gcd(3, 5) == 1, "");
static_assert(gcd(5, 10) == 5, "");
static_assert(gcd(8, 6) == 2, "");
static_assert(gcd(8, 0) == 8, "");
static_assert(gcd(0, 8) == 8, "");
static_assert(gcd(0, 0) == 0, "");

static_assert(lcm(0, 0) == 0, "");
static_assert(lcm(1, 0) == 0, "");
static_assert(lcm(0, 1) == 0, "");
static_assert(lcm(2, 3) == 6, "");
static_assert(lcm(2, 4) == 4, "");

static_assert(fac(0) == 1, "");
static_assert(fac(1) == 1 * 1, "");
static_assert(fac(2) == 1 * 1 * 2, "");
static_assert(fac(3) == 1 * 1 * 2 * 3, "");
static_assert(fac(4) == 1 * 1 * 2 * 3 * 4, "");



constexpr ratio<int> two{4, 2}, three{3};
static_assert(two + three == 5_r, "");
static_assert(two * three < 10_r, "");

static_assert(1_r / 2_r == ratio<int>{1, 2}, "");
} // end namespace sandbox

int main() { }
