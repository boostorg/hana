/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXAMPLE_CPPCON_2014_MATRIX_MATRIX_HPP
#define BOOST_HANA_EXAMPLE_CPPCON_2014_MATRIX_MATRIX_HPP

#include <boost/hana/constant.hpp>
#include <boost/hana/functional/on.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/fwd/comparable.hpp>
#include <boost/hana/fwd/group.hpp>
#include <boost/hana/fwd/monoid.hpp>
#include <boost/hana/fwd/ring.hpp>
#include <boost/hana/tuple.hpp>

#include <utility>


namespace cppcon {
    template <unsigned Rows, unsigned Columns>
    struct Matrix { };

    template <unsigned Rows, unsigned Columns, typename Storage>
    struct matrix_type {
        struct hana { using datatype = Matrix<Rows, Columns>; };

        Storage rows_;
        constexpr auto ncolumns() const
        { return boost::hana::length(boost::hana::front(rows_)); }

        constexpr auto nrows() const
        { return boost::hana::length(rows_); }

        constexpr auto size() const
        { return nrows() * ncolumns(); }

        template <typename I, typename J>
        constexpr decltype(auto) at(I i, J j) const
        { return boost::hana::at(boost::hana::at(rows_, i), j); }
    };

    auto row = boost::hana::make_tuple;

    auto matrix = [](auto&& ...rows) -> decltype(auto) {
        using namespace boost::hana;
        auto storage = make<Tuple>(std::forward<decltype(rows)>(rows)...);
        auto ncolumns = length(front(storage));
        BOOST_HANA_CONSTANT_CHECK(
            all_of(tail(storage), [&](auto const& row) {
                return length(row) == ncolumns;
            })
        );

        return matrix_type<
            sizeof...(rows), value(ncolumns), decltype(storage)
        >{std::move(storage)};
    };

    auto vector = boost::hana::on(matrix, row);


    // More operations
    auto rows = [](auto&& m) -> decltype(auto) {
        return std::forward<decltype(m)>(m).rows_;
    };

    auto transpose = [](auto&& m) -> decltype(auto) {
        return boost::hana::unpack(
            boost::hana::fuse(boost::hana::zip)(rows(std::forward<decltype(m)>(m))),
            matrix
        );
    };

    auto columns = [](auto&& m) -> decltype(auto) {
        return rows(transpose(std::forward<decltype(m)>(m)));
    };

    auto element_wise = [](auto&& f) -> decltype(auto) {
        using namespace boost::hana;
        return [f(std::forward<decltype(f)>(f))](auto&& ...m) -> decltype(auto) {
            return unpack(
                zip_with(partial(zip_with, f),
                    rows(std::forward<decltype(m)>(m))...
                ),
                matrix
            );
        };
    };

    namespace detail {
        auto tuple_scalar_product = [](auto&& u, auto&& v) -> decltype(auto) {
            using namespace boost::hana;
            return sum<>(zip_with(mult,
                std::forward<decltype(u)>(u),
                std::forward<decltype(v)>(v)
            ));
        };
    }
} // end namespace cppcon

#endif // !BOOST_HANA_EXAMPLE_CPPCON_2014_MATRIX_MATRIX_HPP
