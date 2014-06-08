/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core.hpp>
#include <boost/hana/integral.hpp>

#include <utility>
#include <type_traits>


namespace take2 {
    using boost::hana::datatype_t;
    using boost::hana::true_;
    using boost::hana::false_;


    template <typename T, typename U, typename = int>
    constexpr auto has_common_type = false_;

    template <typename T, typename U>
    constexpr auto has_common_type<T, U,
        decltype((true ? std::declval<T>() : std::declval<U>()), int{0})
    > = true_;

    template <typename T, typename U>
    struct Comparable;

    BOOST_HANA_CONSTEXPR_LAMBDA auto equal = [](auto x, auto y) {
        return Comparable<datatype_t<decltype(x)>, datatype_t<decltype(y)>>::
               equal_impl(x, y);
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto not_equal = [](auto x, auto y) {
        return Comparable<datatype_t<decltype(x)>, datatype_t<decltype(y)>>::
               not_equal_impl(x, y);
    };

    struct fall_back_comparable {
        template <typename X, typename Y>
        static constexpr auto equal_impl(X x, Y y,
            std::enable_if_t<
                !std::is_same<X, Y>{} &&
                has_common_type<X, Y>>* = 0)
        {
            using C = std::common_type_t<X, Y>;
            return equal(C{x}, C{y});
        }

        template <typename X, typename Y>
        static constexpr auto equal_impl(X x, Y y,
            std::enable_if_t<
                !std::is_same<X, Y>{} &&
                !has_common_type<X, Y>>* = 0)
        {
            return false_;
        }

        template <typename X, typename Y>
        static constexpr auto equal_impl(X x, Y y,
            std::enable_if_t<
                std::is_same<X, Y>{}>* = 0)
        {
            static_assert((y, void(), false_),
            "trying to compare two objects of an incomparable type");
            return false_;
        }

        template <typename X, typename Y>
        static constexpr auto not_equal_impl(X x, Y y)
        { return !equal_impl(x, y); }
    };


    template <typename T, typename U>
    struct Comparable
        : fall_back_comparable
    { };

    template <template <typename ...> class>
    struct defaults;

    template <>
    struct defaults<Comparable> {
        template <typename T, typename U>
        static constexpr auto equal_impl(T t, U u)
        { return !not_equal(t, u); }

        template <typename T, typename U>
        static constexpr auto not_equal_impl(T t, U u)
        { return !equal(t, u); }
    };

    // fundamental types
#define BOOST_HANA_COMPARE_FUNDAMENTAL(T)                                   \
    template <>                                                             \
    struct Comparable<T, T> : defaults<Comparable> {                        \
        static constexpr auto equal_impl(T x, T y)                          \
        { return x == y; }                                                  \
    };                                                                      \
/**/

    BOOST_HANA_COMPARE_FUNDAMENTAL(bool)
    BOOST_HANA_COMPARE_FUNDAMENTAL(char)
    BOOST_HANA_COMPARE_FUNDAMENTAL(short)
    BOOST_HANA_COMPARE_FUNDAMENTAL(int)
    BOOST_HANA_COMPARE_FUNDAMENTAL(long)
    BOOST_HANA_COMPARE_FUNDAMENTAL(long long)

    BOOST_HANA_COMPARE_FUNDAMENTAL(unsigned char)
    BOOST_HANA_COMPARE_FUNDAMENTAL(unsigned short)
    BOOST_HANA_COMPARE_FUNDAMENTAL(unsigned int)
    BOOST_HANA_COMPARE_FUNDAMENTAL(unsigned long)
    BOOST_HANA_COMPARE_FUNDAMENTAL(unsigned long long)

    BOOST_HANA_COMPARE_FUNDAMENTAL(float)
    BOOST_HANA_COMPARE_FUNDAMENTAL(double)
    BOOST_HANA_COMPARE_FUNDAMENTAL(long double)

#undef BOOST_HANA_COMPARE_FUNDAMENTAL
}

//////////////////////////////////////////////////////////////////////////////
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
using namespace take2;
using boost::hana::int_;
using boost::hana::long_;


struct X { };

int main() {
    BOOST_HANA_STATIC_ASSERT(not_equal(X{}, int{1}));
    BOOST_HANA_STATIC_ASSERT(equal(int_<1>, int{1}));
    BOOST_HANA_STATIC_ASSERT(equal(int_<1>, float{1}));
    BOOST_HANA_STATIC_ASSERT(equal(long_<1>, float{1}));
}

/*
In the above, we use the actual type of objects to perform dispatching.
Consider the following. Let's assume `List` does not define comparison.
Lets also assume we have the following data constructors for creating Lists.

    list(1, 2, 3)
    list_c<int, 1, 2, 3>

Let's assume the actual type of those is different (perhaps list_c provides
some kind of optimization). Then, when comparing

    equal(list(1, 2, 3), list_c<int, 1, 2, 3>)

we get false in the above system because they have different actual types.
Another possibility would be to use the data type instead of the actual type
of objects to perform that dispatching, and then we would get an error saying
these two lists have the same data type but they are not comparable. For
Integrals, it could look like

    int{n} has data type RuntimeIntegral
    int_<n> has data type Integral

And then we say that the common_datatype between RuntimeIntegral and Integral
is RuntimeIntegral, and provide the conversion.


### Another idea
It turns out that the idea below does not work (the function from a Datatype
to its Typeclass(es) is multi-valued), but if it worked it would be equivalent
to the 2-level predicate-based dispatching sketched in sandbox/dispatch.cpp.

------------------------

Would it make sense to have 2 levels of dispatching (at least for Comparable):

    (level 1) Comparable<Datatype1, Datatype2>
    (level 2) Comparable<Typeclass1, Typeclass2>

So we can write

    Comparable<List, List> // my efficient implementation
    Comparable<Iterable, Iterable> // default implementation for iterables

Note that ambiguities can arise if we then provide

    Comparable<OtherTypeclass1, OtherTypeclass2>

and the data types are also instances of OtherTypeclass{1,2} respectively.
The failure caused by ambiguity might be desirable though.
 */
