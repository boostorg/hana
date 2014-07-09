#include <boost/hana/ext/boost/fusion.hpp>
#include <boost/fusion/tuple.hpp>
auto list = [](auto ...xs) {
    return boost::fusion::make_tuple(xs...);
};