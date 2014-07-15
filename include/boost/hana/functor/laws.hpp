/*!
@file
Defines `boost::hana::Functor::laws`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FUNCTOR_LAWS_HPP
#define BOOST_HANA_FUNCTOR_LAWS_HPP

#include <boost/hana/functor/functor.hpp>

#include <boost/hana/comparable/comparable.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/logical.hpp>


namespace boost { namespace hana {
    //! @details
    //! Instances of `Functor` must satisfy the following laws:
    //! @code
    //!     fmap id == id
    //!     fmap (f . g) == fmap f . fmap g
    //! @endcode
    struct Functor::laws {
        template <typename Functor_, typename F, typename G>
        static constexpr auto check(Functor_ functor, F f, G g) {
            return and_(
                equal(fmap(id, functor), functor),
                equal(fmap(compose(f, g), functor), fmap(f, fmap(g, functor)))
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FUNCTOR_LAWS_HPP
