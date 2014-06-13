<%= render('_main.erb') {
    <<-EOS
    #include <boost/hana/detail/constexpr.hpp>
    #include <boost/hana/list.hpp>
    BOOST_HANA_CONSTEXPR_LAMBDA auto partition = [](auto pred, auto xs) {
        auto not_pred = [=](auto x) { return !pred(x); };
        return boost::hana::list(
            boost::hana::filter(pred, xs),
            boost::hana::filter(not_pred, xs)
        );
    };
    EOS
} %>