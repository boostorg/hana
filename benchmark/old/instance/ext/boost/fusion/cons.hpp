<%
def includes(n)
    return <<-EOS
        #include <boost/fusion/container/list/cons.hpp>
        #include <boost/hana/ext/boost/fusion.hpp>
    EOS
end

def list(xs_t, xs)
    return xs_t.foldr('boost::fusion::nil_') { |head, tail|
        "boost::fusion::cons<#{head}, #{tail}>"
    } + '{}'
end
%>
