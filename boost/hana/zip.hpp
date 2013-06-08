
#ifndef BOOST_HANA_ZIP_HPP
#define BOOST_HANA_ZIP_HPP

#include <boost/hana/args.hpp>
#include <boost/hana/at.hpp>
#include <boost/hana/irange.hpp>
#include <boost/hana/lambda.hpp>
#include <boost/hana/length.hpp>
#include <boost/hana/map.hpp>


namespace boost { namespace hana {
namespace zip_detail {
    static auto const smallest_sequence_in =
        filter(...)
    ;
}

// _args == sequence containing all the arguments passed to the function
static auto const zip = lambda(
    map(irange(0, length(zip_detail::smallest_sequence_in(_args))),
        lambda(map(_args, at(_1, from_outer_scope(_args)))) // we'll need something similar to DeBruijn bind, or let[] declarations or something similar
    )
);
}} // end namespace boost::hana

#endif // !BOOST_HANA_ZIP_HPP
