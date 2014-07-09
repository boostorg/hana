#include <boost/hana/ext/boost/fusion.hpp>
#include <boost/fusion/container/generation/make_vector.hpp>
auto list = [](auto ...xs) {
    return boost::fusion::make_vector(xs...);
};