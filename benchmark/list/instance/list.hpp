#include <boost/hana/list.hpp>
auto list = [](auto ...xs) {
    return boost::hana::list(xs...);
};