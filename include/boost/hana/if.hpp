/*!
@file
Defines `boost::hana::if_`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_IF_HPP
#define BOOST_HANA_IF_HPP

#include <boost/hana/fwd/if.hpp>

#include <boost/hana/concept/logical.hpp>
#include <boost/hana/core/dispatch.hpp>
#include <boost/hana/eval_if.hpp>
#include <boost/hana/functional/always.hpp>


namespace boost { namespace hana {
    //! @cond
    template <typename Cond, typename Then, typename Else>
    constexpr decltype(auto) if_t::operator()(Cond&& cond, Then&& then, Else&& else_) const {
        using Bool = typename hana::tag_of<Cond>::type;
        using If = BOOST_HANA_DISPATCH_IF(if_impl<Bool>,
            Logical<Bool>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(Logical<Bool>::value,
        "hana::if_(cond, then, else) requires 'cond' to be a Logical");
    #endif

        return If::apply(static_cast<Cond&&>(cond),
                         static_cast<Then&&>(then),
                         static_cast<Else&&>(else_));
    }
    //! @endcond

    template <typename L, bool condition>
    struct if_impl<L, when<condition>> : default_ {
        //! @todo By using `always` here, we create a copy of both `t`
        //! and `e`, which is not very smart.
        template <typename C, typename T, typename E>
        static constexpr decltype(auto) apply(C&& c, T&& t, E&& e) {
            return hana::eval_if(static_cast<C&&>(c),
                hana::always(static_cast<T&&>(t)),
                hana::always(static_cast<E&&>(e))
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_IF_HPP
