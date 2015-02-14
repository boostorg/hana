/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_AUTO_CONSTANT_HPP
#define BOOST_HANA_TEST_TEST_AUTO_CONSTANT_HPP

#include <boost/hana/assert.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/constant.hpp>
#include <boost/hana/core/common.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/detail/canonical_constant.hpp>
#include <boost/hana/detail/std/is_same.hpp>
#include <boost/hana/enumerable.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/group.hpp>
#include <boost/hana/integral_domain.hpp>
#include <boost/hana/orderable.hpp>
#include <boost/hana/type.hpp>

#include <test/auto/base.hpp>
#include <test/auto/comparable.hpp>
#include <test/auto/enumerable.hpp>
#include <test/auto/group.hpp>
#include <test/auto/integral_domain.hpp>
#include <test/auto/monoid.hpp>
#include <test/auto/orderable.hpp>


namespace boost { namespace hana { namespace test {
    template <typename C, typename T>
    struct wrap_arbitrary_constant {
        static constexpr T get()
        { return boost::hana::value(C{}); }
        struct hana { using datatype = detail::CanonicalConstant<T>; };
    };

    template <typename C>
    auto laws<Constant, C> = [] {
        static_assert(models<Constant(C)>{}, "");

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

        // Comparable
        {
            eval_if(is_an<Comparable, typename C::value_type>,
                [=](auto _) {
                    using Comp = typename decltype(+_(type<hana::Comparable>))::type;
                    laws<Comp, C>();

                    _(for_each)(objects<C>, [](auto x) {
                        for_each(objects<C>, [=](auto y) {
                            BOOST_HANA_CHECK(
                                value(equal(x, y)) ^iff^ equal(value(x), value(y))
                            );
                        });
                    });
                },
                [](auto) {}
            );
        }

        // Orderable
        {
            eval_if(is_an<Orderable, typename C::value_type>,
                [=](auto _) {
                    using Ord = typename decltype(+_(type<hana::Orderable>))::type;
                    laws<Ord, C>();

                    _(for_each)(objects<C>, [](auto x) {
                        for_each(objects<C>, [=](auto y) {
                            BOOST_HANA_CHECK(
                                value(less(x, y)) ^iff^ less(value(x), value(y))
                            );
                        });
                    });
                },
                [](auto) {}
            );
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

        // Monoid
        {
            eval_if(is_a<Monoid, typename C::value_type>,
                [=](auto _) {
                    using Monoid = typename decltype(+_(type<hana::Monoid>))::type;
                    laws<Monoid, C>();

                    BOOST_HANA_CHECK(equal(
                        value(_(zero<C>)()),
                        _(zero<typename C::value_type>)()
                    ));

                    _(for_each)(objects<C>, [](auto x) {
                        for_each(objects<C>, [=](auto y) {
                            BOOST_HANA_CHECK(equal(
                                plus(value(x), value(y)),
                                value(plus(x, y))
                            ));
                        });
                    });
                },
                [](auto) {}
            );
        }

        // Group
        {
            eval_if(is_a<Group, typename C::value_type>,
                [=](auto _) {
                    using Group = typename decltype(+_(type<hana::Group>))::type;
                    laws<Group, C>();

                    _(for_each)(objects<C>, [](auto x) {
                        BOOST_HANA_CHECK(equal(
                            negate(value(x)),
                            value(negate(x))
                        ));

                        for_each(objects<C>, [=](auto y) {
                            BOOST_HANA_CHECK(equal(
                                minus(value(x), value(y)),
                                value(minus(x, y))
                            ));
                        });
                    });
                },
                [](auto) {}
            );
        }

        // Ring
        {
            eval_if(is_a<Ring, typename C::value_type>,
                [=](auto _) {
                    using Ring = typename decltype(+_(type<hana::Ring>))::type;
                    laws<Ring, C>();

                    BOOST_HANA_CHECK(equal(
                        value(_(one<C>)()),
                        _(one<typename C::value_type>)()
                    ));

                    _(for_each)(objects<C>, [](auto x) {
                        for_each(objects<C>, [=](auto y) {
                            BOOST_HANA_CHECK(equal(
                                mult(value(x), value(y)),
                                value(mult(x, y))
                            ));
                        });
                    });
                },
                [](auto) {}
            );
        }

        // IntegralDomain
        {
            eval_if(is_an<IntegralDomain, typename C::value_type>,
                [=](auto _) {
                    using ID = typename decltype(+_(type<hana::IntegralDomain>))::type;
                    laws<ID, C>();

                    _(for_each)(objects<C>, [=](auto x) {
                        auto nonzero = partial(not_equal, _(zero<C>)());
                        for_each(_(filter)(objects<C>, nonzero), [=](auto y) {
                            BOOST_HANA_CHECK(equal(
                                quot(value(x), value(y)),
                                value(quot(x, y))
                            ));

                            BOOST_HANA_CHECK(equal(
                                mod(value(x), value(y)),
                                value(mod(x, y))
                            ));
                        });
                    });
                },
                [](auto) {}
            );
        }

        // Logical
        {
            eval_if(is_a<Logical, typename C::value_type>,
                [=](auto _) {
                    using Logical = typename decltype(+_(type<hana::Logical>))::type;
                    laws<Logical, C>();

                    _(for_each)(objects<C>, [=](auto x) {
                        BOOST_HANA_CHECK(equal(
                            value(not_(x)),
                            not_(value(x))
                        ));

                        _(for_each)(objects<C>, [=](auto y) {
                            BOOST_HANA_CHECK(equal(
                                value(and_(x, y)),
                                and_(value(x), value(y))
                            ));

                            BOOST_HANA_CHECK(equal(
                                value(or_(x, y)),
                                or_(value(x), value(y))
                            ));
                        });
                    });
                },
                [](auto) {}
            );
        }
    };
}}} // end namespace boost::hana::test

#endif // !BOOST_HANA_TEST_TEST_AUTO_CONSTANT_HPP
