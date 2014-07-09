#include <boost/hana/ext/boost/tuple.hpp>
#include <boost/tuple/tuple.hpp>
auto list = [](auto ...xs) {
    return boost::make_tuple(xs...);
};