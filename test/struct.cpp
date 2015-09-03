/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/optional.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/concept/struct.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/base.hpp>
#include <laws/comparable.hpp>
#include <laws/foldable.hpp>
#include <laws/searchable.hpp>

#include <test/minimal_product.hpp>
#include <test/seq.hpp>
using namespace boost::hana;


template <int i = 0>
struct undefined { };

using test::ct_eq;

struct move_only {
    move_only() = default;
    move_only(move_only const&) = delete;
    move_only(move_only&&) = default;
};

template <int N>
struct MinimalStruct;

template <typename ...Members>
struct minimal_struct_t {
    tuple<Members...> members;
    using hana_tag = MinimalStruct<sizeof...(Members)>;
};

struct obj_t {
    template <typename ...Members>
    constexpr minimal_struct_t<Members...> operator()(Members ...members) const {
        return {{static_cast<Members&&>(members)...}};
    }
};
constexpr obj_t obj{};

namespace boost { namespace hana {
    template <int N>
    struct accessors_impl<MinimalStruct<N>> {
        static BOOST_HANA_CONSTEXPR_LAMBDA auto apply() {
            return unpack(range_c<int, 0, N>, [](auto ...k) {
                return make_tuple(make_pair(k,
                    [=](auto&& strct) -> decltype(auto) {
                        using Strct = decltype(strct);
                        return at(static_cast<Strct&&>(strct).members, k);
                    }
                )...);
            });
        }
    };
}}

int main() {
    constexpr auto prod = test::minimal_product; (void)prod;

    auto eq0 = make_tuple(obj());
    auto eq1 = make_tuple(
        obj(ct_eq<0>{}), obj(ct_eq<1>{}), obj(ct_eq<2>{})
    );
    auto eq2 = make_tuple(
        obj(ct_eq<0>{}, ct_eq<0>{}),
        obj(ct_eq<0>{}, ct_eq<1>{}),
        obj(ct_eq<1>{}, ct_eq<0>{}),
        obj(ct_eq<1>{}, ct_eq<1>{}),
        obj(ct_eq<0>{}, ct_eq<2>{}),
        obj(ct_eq<2>{}, ct_eq<3>{})
    );

#if BOOST_HANA_TEST_PART == 1
    //////////////////////////////////////////////////////////////////////////
    // members
    //////////////////////////////////////////////////////////////////////////
    {
        BOOST_HANA_CONSTANT_CHECK(equal(
            members(obj()),
            test::seq()
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            members(obj(ct_eq<0>{})),
            test::seq(ct_eq<0>{})
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            members(obj(ct_eq<0>{}, ct_eq<1>{})),
            test::seq(ct_eq<0>{}, ct_eq<1>{})
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            members(obj(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{})),
            test::seq(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{})
        ));

        // make sure it works with move only types
        auto z1 = members(obj(move_only{})); (void)z1;
        auto z2 = members(obj(move_only{}, move_only{})); (void)z2;
    }

    //////////////////////////////////////////////////////////////////////////
    // keys
    //////////////////////////////////////////////////////////////////////////
    {
        BOOST_HANA_CONSTANT_CHECK(equal(
            keys(obj()),
            test::seq()
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            keys(obj(undefined<0>{})),
            test::seq(int_c<0>)
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            keys(obj(undefined<0>{}, undefined<1>{})),
            test::seq(int_c<0>, int_c<1>)
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            keys(obj(undefined<0>{}, undefined<1>{}, undefined<2>{})),
            test::seq(int_c<0>, int_c<1>, int_c<2>)
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            keys(obj(undefined<0>{}, undefined<1>{}, undefined<2>{}, undefined<3>{})),
            test::seq(int_c<0>, int_c<1>, int_c<2>, int_c<3>)
        ));
    }

    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    {
        // equal
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                obj(),
                obj()
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                obj(ct_eq<0>{}),
                obj(ct_eq<0>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                obj(ct_eq<0>{}),
                obj(ct_eq<1>{})
            )));

            BOOST_HANA_CONSTANT_CHECK(equal(
                obj(ct_eq<0>{}, ct_eq<1>{}),
                obj(ct_eq<0>{}, ct_eq<1>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                obj(ct_eq<1>{}, ct_eq<0>{}),
                obj(ct_eq<0>{}, ct_eq<1>{})
            )));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                obj(ct_eq<0>{}, ct_eq<99>{}),
                obj(ct_eq<0>{}, ct_eq<1>{})
            )));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                obj(ct_eq<99>{}, ct_eq<1>{}),
                obj(ct_eq<0>{}, ct_eq<1>{})
            )));
        }

        // laws
        test::TestComparable<MinimalStruct<0>>{eq0};
        test::TestComparable<MinimalStruct<1>>{eq1};
        test::TestComparable<MinimalStruct<2>>{eq2};
    }

#elif BOOST_HANA_TEST_PART == 2
    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    {
        test::ct_eq<999> s{};
        test::_injection<0> f{};

        // fold_left
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                fold_left(obj(), s, undefined<>{}),
                s
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                fold_left(obj(ct_eq<0>{}), s, f),
                f(s, prod(int_c<0>, ct_eq<0>{}))
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                fold_left(obj(ct_eq<0>{}, ct_eq<1>{}), s, f),
                f(f(s, prod(int_c<0>, ct_eq<0>{})), prod(int_c<1>, ct_eq<1>{}))
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                fold_left(obj(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{}), s, f),
                f(f(f(s, prod(int_c<0>, ct_eq<0>{})),
                         prod(int_c<1>, ct_eq<1>{})),
                         prod(int_c<2>, ct_eq<2>{}))
            ));

            // fold_left with move-only members
            fold_left(obj(move_only{}), 0, [](int, auto) { return 0; });
            fold_left(obj(move_only{}, move_only{}), 0, [](int, auto) { return 0; });
        }

        // fold_right
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                fold_right(obj(), s, undefined<>{}),
                s
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                fold_right(obj(ct_eq<0>{}), s, f),
                f(prod(int_c<0>, ct_eq<0>{}), s)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                fold_right(obj(ct_eq<0>{}, ct_eq<1>{}), s, f),
                f(prod(int_c<0>, ct_eq<0>{}), f(prod(int_c<1>, ct_eq<1>{}), s))
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                fold_right(obj(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{}), s, f),
                f(prod(int_c<0>, ct_eq<0>{}),
                    f(prod(int_c<1>, ct_eq<1>{}),
                        f(prod(int_c<2>, ct_eq<2>{}), s)))
            ));

            // fold_right with move-only members
            fold_right(obj(move_only{}), 0, [](auto, int) { return 0; });
            fold_right(obj(move_only{}, move_only{}), 0, [](auto, int) { return 0; });
        }

        // unpack
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(obj(), f),
                f()
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(obj(ct_eq<0>{}), f),
                f(prod(int_c<0>, ct_eq<0>{}))
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(obj(ct_eq<0>{}, ct_eq<1>{}), f),
                f(prod(int_c<0>, ct_eq<0>{}), prod(int_c<1>, ct_eq<1>{}))
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(obj(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{}), f),
                f(prod(int_c<0>, ct_eq<0>{}), prod(int_c<1>, ct_eq<1>{}),
                                              prod(int_c<2>, ct_eq<2>{}))
            ));
        }

        // laws
        test::TestFoldable<MinimalStruct<0>>{eq0};
        test::TestFoldable<MinimalStruct<1>>{eq1};
        test::TestFoldable<MinimalStruct<2>>{eq2};
    }

#elif BOOST_HANA_TEST_PART == 3
    //////////////////////////////////////////////////////////////////////////
    // Searchable
    //////////////////////////////////////////////////////////////////////////
    {
        // any_of
        {
            BOOST_HANA_CONSTANT_CHECK(not_(any_of(
                obj(), undefined<>{}
            )));

            BOOST_HANA_CONSTANT_CHECK(any_of(
                obj(undefined<0>{}), equal.to(int_c<0>)
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(any_of(
                obj(undefined<0>{}), equal.to(int_c<1>)
            )));

            BOOST_HANA_CONSTANT_CHECK(any_of(
                obj(undefined<0>{}, undefined<1>{}), equal.to(int_c<0>)
            ));
            BOOST_HANA_CONSTANT_CHECK(any_of(
                obj(undefined<0>{}, undefined<1>{}), equal.to(int_c<1>)
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(any_of(
                obj(undefined<0>{}, undefined<1>{}), equal.to(int_c<2>)
            )));
        }

        // find_if
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                find_if(obj(), undefined<>{}),
                nothing
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                find_if(obj(ct_eq<0>{}), equal.to(int_c<0>)),
                just(ct_eq<0>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                find_if(obj(undefined<1>{}), equal.to(int_c<1>)),
                nothing
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                find_if(obj(ct_eq<0>{}, ct_eq<1>{}), equal.to(int_c<0>)),
                just(ct_eq<0>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                find_if(obj(ct_eq<0>{}, ct_eq<1>{}), equal.to(int_c<1>)),
                just(ct_eq<1>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                find_if(obj(undefined<0>{}, undefined<1>{}), equal.to(int_c<2>)),
                nothing
            ));
        }

        // laws
        test::TestSearchable<MinimalStruct<0>>{eq0, make_tuple()};
        test::TestSearchable<MinimalStruct<1>>{eq1, make_tuple(int_c<0>)};
        test::TestSearchable<MinimalStruct<2>>{eq2, make_tuple(int_c<0>, int_c<1>)};
    }
#endif
}
