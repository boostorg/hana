/*!
@file
Adapts `boost::fusion::tuple` for use with Hana.

In the current version of Boost.Fusion, `boost::fusion::tuple` is basically
an alias to `boost::fusion::vector`, so both data types share the same
implementation in Hana.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_BOOST_FUSION_TUPLE_HPP
#define BOOST_HANA_EXT_BOOST_FUSION_TUPLE_HPP

#include <boost/hana/ext/boost/fusion/vector.hpp>


namespace boost { namespace hana {
    namespace ext { namespace boost { namespace fusion {
        // In the current version of Boost.Fusion, `boost::fusion::tuple` is
        // basically an alias to `boost::fusion::vector`, hence the alias.
        using tuple_tag = vector_tag;
    }}}
}}

#endif // !BOOST_HANA_EXT_BOOST_FUSION_TUPLE_HPP
