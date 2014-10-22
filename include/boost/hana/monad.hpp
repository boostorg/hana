/*!
@file
Defines `boost::hana::Monad`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_MONAD_HPP
#define BOOST_HANA_MONAD_HPP

#include <boost/hana/fwd/monad.hpp>

#include <boost/hana/applicative.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/detail/std/enable_if.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/functor.hpp>


namespace boost { namespace hana {
    namespace operators {
        //! Equivalent to `bind`.
        //! @relates boost::hana::Monad
        //!
        //! @note
        //! This was preferred over `>>=` because `>>=` is right associative
        //! in C++, which makes it impossible to chain computations.
        template <typename M, typename F, typename = typename detail::std::enable_if<
            enable_operators<Monad, datatype_t<M>>::value
        >::type>
        constexpr decltype(auto) operator|(M&& m, F&& f) {
            return bind(
                detail::std::forward<decltype(m)>(m),
                detail::std::forward<decltype(f)>(f)
            );
        }
    }

    namespace monad_detail {
        template <typename M>
        struct common {
            template <typename M1, typename M2>
            static constexpr decltype(auto) then_impl(M1&& m1, M2&& m2) {
                return bind(
                    detail::std::forward<M1>(m1),
                    always(detail::std::forward<M2>(m2))
                );
            }

            template <typename F>
            static constexpr auto tap_impl(F&& f) {
                return [f(detail::std::forward<F>(f))](auto&& x) -> decltype(auto) {
                    f(x);
                    return lift<M>(detail::std::forward<decltype(x)>(x));
                };
            }
        };
    }

    //! Minimal complete definition: `bind`
    template <typename M>
    struct Monad::bind_mcd : monad_detail::common<M> {
        template <typename MM>
        static constexpr decltype(auto) flatten_impl(MM&& monad)
        { return bind(detail::std::forward<MM>(monad), id); }
    };

    //! Minimal complete definition: `flatten`
    template <typename M>
    struct Monad::flatten_mcd : monad_detail::common<M> {
        template <typename Mon, typename F>
        static constexpr decltype(auto) bind_impl(Mon&& monad, F&& f) {
            return flatten(
                fmap(
                    detail::std::forward<Mon>(monad),
                    detail::std::forward<F>(f)
                )
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_MONAD_HPP
