/*!
@file
Defines `boost::hana::Foldable`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CONCEPT_FOLDABLE_HPP
#define BOOST_HANA_CONCEPT_FOLDABLE_HPP

#include <boost/hana/fwd/concept/foldable.hpp>

#include <boost/hana/concept/sequence.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>

#include <boost/hana/count.hpp>
#include <boost/hana/count_if.hpp>
#include <boost/hana/fold.hpp>
#include <boost/hana/fold_left.hpp>
#include <boost/hana/fold_right.hpp>
#include <boost/hana/for_each.hpp>
#include <boost/hana/fuse.hpp>
#include <boost/hana/length.hpp>
#include <boost/hana/maximum.hpp>
#include <boost/hana/minimum.hpp>
#include <boost/hana/monadic_fold_left.hpp>
#include <boost/hana/monadic_fold_right.hpp>
#include <boost/hana/product.hpp>
#include <boost/hana/reverse_fold.hpp>
#include <boost/hana/size.hpp>
#include <boost/hana/sum.hpp>
#include <boost/hana/unpack.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // models
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct models_impl<Foldable, T> {
        static constexpr bool value = !is_default<fold_left_impl<T>>::value ||
                                      !is_default<unpack_impl<T>>::value;
    };

    //////////////////////////////////////////////////////////////////////////
    // Automatic Foldable -> Sequence conversion
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename F>
    struct to_impl<S, F, when<_models<Sequence, S>::value &&
                              _models<Foldable, F>::value>>
        : embedding<_models<Sequence, F>::value>
    {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs)
        { return hana::unpack(static_cast<Xs&&>(xs), hana::make<S>); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_CONCEPT_FOLDABLE_HPP
