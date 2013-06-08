
#ifndef BOOST_HANA_PAIR_HPP
#define BOOST_HANA_PAIR_HPP

#include <boost/hana/vector.hpp>


namespace boost { namespace hana {
    /*!
     *
     */
    template <typename First, typename Second>
    using pair = vector<First, Second>;
}} // end namespace boost::hana

#endif // !BOOST_HANA_PAIR_HPP
