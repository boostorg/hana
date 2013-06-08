
#ifndef BOOST_HANA_FILTER_HPP
#define BOOST_HANA_FILTER_HPP

namespace boost { namespace hana {

// seq(_args) and pred(_args) fetch the seq and pred named arguments from _args.
// we should have a default for this function, i.e. filter(some_sequence, some_pred)
// is equivalent to filter(seq = some_sequence, pred = some_pred).

static auto const filter = lambda(
    sequence_compatible_with(_args[seq])(
        _args[seq] | _args[pred] // the | is a filter, like set comprehensions
    )
);
}} // end namespace boost::hana

#endif // !BOOST_HANA_FILTER_HPP
