/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/when.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>
#include <boost/hana/type_list.hpp>
namespace hana = boost::hana;


// This is an attempt to break cyclic dependencies that arise when several
// minimal complete definitions are possible. For example,
//
//      method1 => method2
//      method2 => method1
//
// where `=>` means "provides". This will cause the partial specializations
// to recurse indefinitely. What we do here is keep track of the methods that
// were seen so far and disable (by hand) the specializations that would
// cause a cycle.
//
// This works, but has drawbacks:
//  1. likely inefficient
//  2. a lot of boilerplate for each method and method specialization
//
//
// By the way, the problem can be formally stated as finding a path ending
// at a vertex without out-edges in a graph. The vertices are methods and
// an edge from `a` to `b` means that method `a` provides method `b`. The
// difficulty is not to get caught in cycles, hence the need to remember
// all the nodes seen so far.

template <template <typename ...> class f>
struct quote;

template <typename g, typename ...f>
auto contains = hana::elem(hana::type_list<f...>, hana::type<g>);

template <typename ...f>
struct seen {
    template <typename ...g>
    using append = seen<f..., g...>;

    template <typename g>
    struct contains {
        static constexpr bool value = hana::value(::contains<g, f...>);
    };
};



using hana::when;

template <typename T, typename Seen = seen<>, typename = void>
struct fmap_impl : fmap_impl<T, Seen, when<true>> { };

template <typename T, typename Seen, bool condition>
struct fmap_impl<T, Seen, when<condition>> {
    static constexpr bool is_implemented = false;
};

template <typename T, typename Seen = seen<>, typename = void>
struct adjust_impl : adjust_impl<T, Seen, when<true>> { };

template <typename T, typename Seen, bool condition>
struct adjust_impl<T, Seen, when<condition>> {
    static constexpr bool is_implemented = false;
};

template <typename T, typename Seen = seen<>, typename = void>
struct ap_impl : ap_impl<T, Seen, when<true>> { };

template <typename T, typename Seen, bool condition>
struct ap_impl<T, Seen, when<condition>> {
    static constexpr bool is_implemented = false;
};



namespace detail {
    template <typename T, typename Seen, bool = !Seen::template contains<quote<adjust_impl>>::value>
    struct fmap_impl_ok {
        static constexpr bool value = false;
    };

    template <typename T, typename Seen>
    struct fmap_impl_ok<T, Seen, true> {
        static constexpr bool value = adjust_impl<
            T, typename Seen::template append<quote<fmap_impl>>
        >::is_implemented;
    };
}

template <typename T, typename Seen>
struct fmap_impl<T, Seen, when<detail::fmap_impl_ok<T, Seen>::value>> {
    static constexpr bool is_implemented = true;
};



namespace detail {
    template <typename T, typename Seen, bool = !Seen::template contains<quote<fmap_impl>>::value>
    struct adjust_impl_ok {
        static constexpr bool value = false;
    };

    template <typename T, typename Seen>
    struct adjust_impl_ok<T, Seen, true> {
        static constexpr bool value = fmap_impl<
            T, typename Seen::template append<quote<adjust_impl>>
        >::is_implemented;
    };
}

template <typename T, typename Seen>
struct adjust_impl<T, Seen, when<detail::adjust_impl_ok<T, Seen>::value>> {
    static constexpr bool is_implemented = true;
};

template <typename T, typename Seen>
struct fmap_impl<T, Seen, when<ap_impl<T, Seen>::is_implemented>> {
    static constexpr bool is_implemented = true;
};



template <typename Seen>
struct ap_impl<hana::Tuple, Seen> {
    static constexpr bool is_implemented = true;
};

template <typename Seen>
struct adjust_impl<hana::Tuple, Seen> {
    static constexpr bool is_implemented = true;
};

template <typename Seen>
struct fmap_impl<hana::Tuple, Seen> {
    static constexpr bool is_implemented = true;
};



int main() {
    using x = seen<quote<fmap_impl>>::append<quote<adjust_impl>>;
    static_assert(!seen<quote<fmap_impl>>::contains<quote<adjust_impl>>::value, "");
    static_assert(seen<quote<fmap_impl>, quote<adjust_impl>>::contains<quote<adjust_impl>>::value, "");

    static_assert(!fmap_impl<int>::is_implemented, "");
    static_assert(!adjust_impl<int>::is_implemented, "");
    static_assert(!ap_impl<int>::is_implemented, "");

    static_assert(fmap_impl<hana::Tuple>::is_implemented, "");
    static_assert(adjust_impl<hana::Tuple>::is_implemented, "");
}
