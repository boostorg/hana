/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_LAWS_CONSTANT_HPP
#define BOOST_HANA_TEST_LAWS_CONSTANT_HPP

#include <boost/hana/assert.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/detail/std/is_same.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/logical.hpp>

#include <laws/base.hpp>
#include <laws/comparable.hpp>
#include <laws/enumerable.hpp>
#include <laws/group.hpp>
#include <laws/integral_domain.hpp>
#include <laws/logical.hpp>
#include <laws/monoid.hpp>
#include <laws/orderable.hpp>
#include <laws/ring.hpp>


namespace boost { namespace hana { namespace test {
    template <typename C, typename = when<true>>
    struct TestConstant {
        static_assert(models<Constant, C>{}, "");
        using T = typename C::value_type;

        template <typename X>
        struct wrap_arbitrary_constant {
            static constexpr T get()
            { return boost::hana::value(X{}); }
            struct hana { using datatype = detail::CanonicalConstant<T>; };
        };

        template <typename Xs>
        TestConstant(Xs xs) {
            foreach(xs, [](auto c) {

                // constexpr-ness of hana::value(c)
                constexpr auto must_be_constexpr1 = hana::value(c);
                constexpr auto must_be_constexpr2 = hana::value<decltype(c)>();
                (void)must_be_constexpr1;
                (void)must_be_constexpr2;

                // consistency of C::value_type
                static_assert(detail::std::is_same<
                    T,
                    datatype_t<decltype(hana::value(c))>
                >{}, "");

                // conversion to the underlying data type
                BOOST_HANA_CHECK(equal(
                    to<T>(c),
                    make<T>(hana::value(c))
                ));

                // conversion from an arbitrary Constant
                (void)to<C>(wrap_arbitrary_constant<decltype(c)>{});
                static_assert(is_embedded<detail::CanonicalConstant<T>, C>{}, "");

            });

            // provided common data type
            {
                static_assert(detail::std::is_same<common_t<C, C>, C>{}, "");

                static_assert(detail::std::is_same<
                    common_t<C, detail::CanonicalConstant<T>>,
                    detail::CanonicalConstant<T>
                >{}, "");

                static_assert(detail::std::is_same<
                    common_t<detail::CanonicalConstant<T>, C>,
                    detail::CanonicalConstant<T>
                >{}, "");
            }
        }
    };
}}} // end namespace boost::hana::test

#endif // !BOOST_HANA_TEST_LAWS_CONSTANT_HPP
