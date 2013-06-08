
#ifndef BOOST_HANA_VECTOR_HPP
#define BOOST_HANA_VECTOR_HPP

#include <boost/hana/begin.hpp>


namespace boost { namespace hana {
    template <typename ...>
    class vector { };

    template <typename Head, typename ...Tail>
    class vector<Head, Tail...> : vector<Tail...> { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_VECTOR_HPP
