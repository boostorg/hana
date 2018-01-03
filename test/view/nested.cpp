// Copyright Jason Rice 2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/basic_tuple.hpp>
#include <boost/hana/concat.hpp>
#include <boost/hana/drop_back.hpp>
#include <boost/hana/drop_front.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/flatten.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/reverse.hpp>
#include <boost/hana/slice.hpp>
#include <boost/hana/view.hpp>

#include <memory>

#define CHECK_EQUAL(X, Y) BOOST_HANA_RUNTIME_CHECK(X == Y)

namespace hana = boost::hana;

namespace {
    template <int i>
    struct in_place {
        int value;

        in_place()
            : value(i)
        { }

        bool operator==(in_place<i> const& x) const {
            return &x == this && value == x.value;
        }
    };

    template <int i>
    struct injected_in_place
    {
        in_place<i> const& value;

        bool operator==(injected_in_place<i> const& x) const {
            return value == x.value;
        }
    };

    template <int i>
    inline injected_in_place<i> make_injected_in_place(in_place<i> const& x) {
        return {x};
    }
}


int main() {
    {
        // identity, identity, joined, sliced
        using Xs = hana::basic_tuple<in_place<0>, in_place<1>, in_place<3>>;
        using Ys = hana::basic_tuple<in_place<4>, in_place<5>>;
        auto xs = std::unique_ptr<Xs>(new Xs{});
        auto ys = std::unique_ptr<Ys>(new Ys{});
        auto xs_view = ([&xs] { 
            auto temp = hana::make_view(*xs);
            return hana::make_view(temp);
        })();
        auto ys_view = hana::make_view(*ys);

        auto result = hana::drop_front(hana::concat(xs_view, ys_view));

        CHECK_EQUAL(hana::at_c<0>(result), hana::at_c<1>(*xs));
        CHECK_EQUAL(hana::at_c<1>(result), hana::at_c<2>(*xs));
        CHECK_EQUAL(hana::at_c<2>(result), hana::at_c<0>(*ys));
        CHECK_EQUAL(hana::at_c<3>(result), hana::at_c<1>(*ys));
    }{
        // identity, flattened, transformed
        auto f = [](auto const& x) { return make_injected_in_place(x); };

        using Xs = hana::basic_tuple<
                       hana::basic_tuple<in_place<0>, in_place<1>, in_place<3>>,
                       hana::basic_tuple<in_place<4>, in_place<5>>
                   >;

        auto xs = std::unique_ptr<Xs>(new Xs{});
        auto xs_view = hana::make_view(*xs);

        auto result = hana::transform(hana::flatten(xs_view), f);

        CHECK_EQUAL(hana::at_c<0>(result), f(hana::at_c<0>(hana::at_c<0>(*xs))));
        CHECK_EQUAL(hana::at_c<1>(result), f(hana::at_c<1>(hana::at_c<0>(*xs))));
        CHECK_EQUAL(hana::at_c<2>(result), f(hana::at_c<2>(hana::at_c<0>(*xs))));
        CHECK_EQUAL(hana::at_c<3>(result), f(hana::at_c<0>(hana::at_c<1>(*xs))));
        CHECK_EQUAL(hana::at_c<4>(result), f(hana::at_c<1>(hana::at_c<1>(*xs))));
    }{
        // identity, joined, empty
        using Xs = hana::basic_tuple<in_place<0>, in_place<1>, in_place<3>>;
        using Ys = hana::basic_tuple<in_place<4>, in_place<5>>;
        auto xs = std::unique_ptr<Xs>(new Xs{});
        auto ys = std::unique_ptr<Ys>(new Ys{});
        auto xs_view = hana::make_view(*xs);
        auto ys_view = hana::make_view(*ys);

        auto result = hana::concat(xs_view, hana::concat(hana::empty<hana::view_tag>(), ys_view));

        BOOST_HANA_CONSTANT_CHECK(hana::equal(hana::length(result), hana::size_c<5>));

        CHECK_EQUAL(hana::at_c<0>(result), hana::at_c<0>(*xs));
        CHECK_EQUAL(hana::at_c<1>(result), hana::at_c<1>(*xs));
        CHECK_EQUAL(hana::at_c<2>(result), hana::at_c<2>(*xs));
        CHECK_EQUAL(hana::at_c<3>(result), hana::at_c<0>(*ys));
        CHECK_EQUAL(hana::at_c<4>(result), hana::at_c<1>(*ys));
    }
}
