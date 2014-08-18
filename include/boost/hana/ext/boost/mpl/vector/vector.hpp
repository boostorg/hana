/*!
@file
Forward declares `boost::hana::BoostMplVector`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_BOOST_MPL_VECTOR_VECTOR_HPP
#define BOOST_HANA_EXT_BOOST_MPL_VECTOR_VECTOR_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/detail/std/is_same.hpp>

#include <boost/mpl/sequence_tag.hpp>
#include <boost/mpl/vector.hpp>


namespace boost { namespace hana {
    //! @ingroup group-datatypes
    //! Data type representing Boost.MPL vectors.
    //!
    //! ## Instance of
    //! `Functor`, `Iterable`, `List` and `Comparable`
    //!
    //! ## MPL / Hana cheat sheet
    //! This section presents correspondences between operations with the
    //! Boost.MPL and operations with Boost.Hana. The organization follows
    //! that of the Boost.MPL algorithms.
    //!
    //! ### Iteration algorithms
    //! #### `fold`
    //! @snippet example/ext/boost/mpl/vector/cheatsheet/fold.cpp mpl
    //! @snippet example/ext/boost/mpl/vector/cheatsheet/fold.cpp hana
    //!
    //! @todo
    //! - Finish the cheat sheet.
    //! - Remove the tuple.hpp include.
    //! - Fix documentation of instances.
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    struct BoostMplVector { };
#else
    using BoostMplVector = boost::mpl::sequence_tag<boost::mpl::vector<>>::type;
#endif

    template <typename T>
    struct datatype<T, when<
        detail::std::is_same<
            typename boost::mpl::sequence_tag<T>::type,
            BoostMplVector
        >::value
    >> {
        using type = BoostMplVector;
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_BOOST_MPL_VECTOR_VECTOR_HPP
