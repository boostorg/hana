/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_AUTO_CONSTANT_HPP
#define BOOST_HANA_TEST_TEST_AUTO_CONSTANT_HPP

#include <boost/hana/assert.hpp>
#include <boost/hana/constant.hpp>
#include <boost/hana/core/common.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/detail/canonical_constant.hpp>
#include <boost/hana/detail/std/is_same.hpp>
#include <boost/hana/enumerable.hpp>
#include <boost/hana/type.hpp>

#include <test/auto/base.hpp>
#include <test/auto/enumerable.hpp>


namespace boost { namespace hana { namespace test {
    template <typename C, typename T>
    struct wrap_arbitrary_constant {
        static constexpr T get()
        { return boost::hana::value(C{}); }
        struct hana { using datatype = detail::CanonicalConstant<T>; };
    };

    template <typename C>
    auto laws<Constant, C> = [] {
        // laws
        {
            for_each(objects<C>, [](auto c) {
                constexpr auto must_be_constexpr = hana::value(c);
                (void)must_be_constexpr;


                using T = decltype(hana::value(c));
                static_assert(detail::std::is_same<
                    datatype_t<T>,
                    typename C::value_type
                >{}, "");


                (void)to<C>(wrap_arbitrary_constant<decltype(c), T>{});
                static_assert(is_embedded<detail::CanonicalConstant<T>, C>{}, "");
            });
        }

        // conversion to the underlying data type
        {
            for_each(objects<C>, [](auto c) {
                using Underlying = typename C::value_type;
                BOOST_HANA_CHECK(equal(
                    to<Underlying>(c),
                    make<Underlying>(hana::value(c))
                ));
            });
        }

        // provided common data type
        {
            static_assert(detail::std::is_same<common_t<C, C>, C>{}, "");

            static_assert(detail::std::is_same<
                common_t<C, detail::CanonicalConstant<typename C::value_type>>,
                detail::CanonicalConstant<typename C::value_type>
            >{}, "");
            static_assert(detail::std::is_same<
                common_t<detail::CanonicalConstant<typename C::value_type>, C>,
                detail::CanonicalConstant<typename C::value_type>
            >{}, "");
        }

        // Enumerable
        {
            eval_if(is_an<Enumerable, typename C::value_type>,
                [=](auto _) {
                    using E = typename decltype(+_(type<hana::Enumerable>))::type;
                    laws<E, C>();

                    for_each(_(objects<C>), [](auto c) {
                        BOOST_HANA_CHECK(equal(
                            succ(value(c)),
                            value(succ(c))
                        ));

                        BOOST_HANA_CHECK(equal(
                            pred(value(c)),
                            value(pred(c))
                        ));
                    });
                },
                [](auto) {}
            );
        }
    };
}}} // end namespace boost::hana::test

#endif // !BOOST_HANA_TEST_TEST_AUTO_CONSTANT_HPP
