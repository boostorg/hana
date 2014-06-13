<%= render('_main.erb') {
    <<-EOS
    #include <boost/hana/detail/constexpr.hpp>
    #include <boost/hana/list.hpp>
    BOOST_HANA_CONSTEXPR_LAMBDA auto partition = boost::hana::partition;
    EOS
} %>