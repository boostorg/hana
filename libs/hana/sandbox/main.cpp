#include <boost/fusion/include/fold.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/transform.hpp>
#include <boost/fusion/include/transform_view.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/mpl/integral_c.hpp>
#include <iostream>
#include <type_traits>
#include <utility>



//////////////////////////////////////////////////////////////////////////////
// traits/tag_of
//////////////////////////////////////////////////////////////////////////////
namespace boost { namespace hana { namespace traits {
    template <typename T>
    struct tag_of {
        typedef typename std::remove_reference<T>::type Raw;
        typedef typename Raw::hana::tag type;
    };
}}}


//////////////////////////////////////////////////////////////////////////////
// BOOST_HANA_AUTO_RETURN
//////////////////////////////////////////////////////////////////////////////
#define BOOST_HANA_AUTO_RETURN(...) \
    -> decltype(__VA_ARGS__) { return __VA_ARGS__; }



//////////////////////////////////////////////////////////////////////////////
// intrinsics
//////////////////////////////////////////////////////////////////////////////
namespace boost { namespace hana {
namespace extension {
    template <typename Tag> struct car;
    template <typename Tag> struct cdr;
    template <typename Tag> struct cons;
}

static struct {
    template <typename Sequence>
    auto operator()(Sequence&& sequence) const BOOST_HANA_AUTO_RETURN(
        extension::car<
            typename traits::tag_of<Sequence>::type
        >::call(std::forward<Sequence>(sequence))
    )
} const car{};

static struct {
    template <typename Sequence>
    auto operator()(Sequence&& sequence) const BOOST_HANA_AUTO_RETURN(
        extension::cdr<
            typename traits::tag_of<Sequence>::type
        >::call(std::forward<Sequence>(sequence))
    )
} const cdr{};

static struct {
    template <typename Car, typename Cdr>
    auto operator()(Car&& car, Cdr&& cdr) const BOOST_HANA_AUTO_RETURN(
        extension::cons<
            typename traits::tag_of<Cdr>::type
        >::call(std::forward<Car>(car), std::forward<Cdr>(cdr))
    )
} const cons{};
}}


//////////////////////////////////////////////////////////////////////////////
// algorithms
//////////////////////////////////////////////////////////////////////////////
namespace boost { namespace hana {
static struct {
    template <typename Sequence, typename F>
    auto operator()(Sequence&& sequence, F&& f) const BOOST_HANA_AUTO_RETURN(
        cons(f(car(sequence)), (*this)(cdr(sequence), f))
    )
} const map{};


//////////////////////////////////////////////////////////////////////////////
// cons
//////////////////////////////////////////////////////////////////////////////
namespace boost { namespace hana {
namespace cons_detail {
    struct nil_tag;
    struct nil_type {
        struct hana { typedef nil_tag tag; };
    };

    struct cons_tag;
    template <typename Car, typename Cdr>
    struct cons_type {
        Car car;
        Cdr cdr;
        struct hana { typedef cons_tag tag; };
    };
}

static struct {
    cons_detail::nil_type operator()() const { return {}; }
} const nil_{};

static struct {
    template <typename Car, typename Cdr = cons_detail::nil_type>
    cons_detail::cons_type<
        typename std::remove_reference<Car>::type,
        typename std::remove_reference<Cdr>::type
    > operator()(Car&& car, Cdr&& cdr = nil_()) const {
        return {std::forward<Car>(car), std::forward<Cdr>(cdr)};
    }
} const cons{};


namespace extension {
    template <>
    struct car<cons_detail::cons_tag> {
        template <typename Cons>
        static auto call(Cons&& c) BOOST_HANA_AUTO_RETURN(
            std::forward<Cons>(c).car
        )
    };

    template <>
    struct cdr<cons_detail::cons_tag> {
        template <typename Cons>
        static auto call(Cons&& c) BOOST_HANA_AUTO_RETURN(
            std::forward<Cons>(c).cdr
        )
    };
}


}}


#if 0

//////////////////////////////////////////////////////////////////////////////
// list
//////////////////////////////////////////////////////////////////////////////
namespace boost { namespace hana {
namespace list_detail { struct tag; }

template <typename ...>
struct list {
    struct hana { typedef list_detail::tag tag; };
};

template <typename Head, typename ...Tail>
struct list<Head, Tail...> : list<Tail...> {
    list() = default;

    template <typename Head_, typename ...Tail_>
    explicit list(Head_&& head, Tail_&& ...tail)
        : list<Tail...>{std::forward<Tail>(tail)...},
          element_{std::forward<Head>(head)}
    { }

private:
    typedef Head car_type;
    typedef list<Tail...> cdr_type;

    car_type& car() { return element_; }
    car_type const& car() const { return element_; }

    cdr_type& cdr() { return static_cast<cdr_type&>(*this); }
    cdr_type const& cdr() const { return static_cast<cdr_type const&>(*this); }

    friend struct extension::car<list_detail::tag>;
    friend struct extension::cdr<list_detail::tag>;
    Head element_;
};

namespace extension {
    template <>
    struct car<list_detail::tag> {
        template <typename List>
        static auto call(List&& list) BOOST_HANA_AUTO_RETURN(
            std::forward<List>(list).car()
        )
    };

    template <>
    struct cdr<list_detail::tag> {
        template <typename List>
        static auto call(List&& list) BOOST_HANA_AUTO_RETURN(
            std::forward<List>(list).cdr()
        )
    };
}

}}
#endif


//////////////////////////////////////////////////////////////////////////////
// apply
//////////////////////////////////////////////////////////////////////////////
namespace boost { namespace hana {
static struct {
    template <typename F, typename ...Args>
    void operator()(F&&, Args&& ...) const {

    }
} const apply{};
}}


//////////////////////////////////////////////////////////////////////////////
// lambda
//////////////////////////////////////////////////////////////////////////////
namespace boost { namespace hana {
namespace lambda_detail {
    template <typename Expression>
    struct lambda_expression {
        Expression expr_;

        template <typename ...Args>
        auto operator()(Args&& ...args) const
        -> decltype(apply(expr_, std::forward<Args>(args)...)) {
            return apply(expr_, std::forward<Args>(args)...);
        }
    };
}

static struct {
    template <typename Expression>
    lambda_detail::lambda_expression<Expression&&>
    operator()(Expression&& expression) const {
        return {std::forward<Expression>(expression)};
    }
} const lambda{};
}}


//////////////////////////////////////////////////////////////////////////////
// main
//////////////////////////////////////////////////////////////////////////////

#define SHOW_TYPE(...) do { typedef show<decltype(__VA_ARGS__)>::type _foabadb1123; } while (false)
template <typename> struct show;

// clang++ -stdlib=libc++ -std=c++11 -Wall -Wextra -pedantic -I /usr/lib/c++/v1 -I ~/code/boost-trunk -I ~/code/hana ~/code/hana/libs/hana/sandbox/main.cpp

using namespace boost;

int main() {
    using namespace hana;
    auto _123 = cons(1, cons(2, cons(3)));
    car(_123);
    cdr(_123);
    std::cout << car(map(_123, [](int i) { return (std::cout << i << ' '), i + 1; }));
}
