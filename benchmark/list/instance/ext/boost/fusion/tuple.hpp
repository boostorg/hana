<%
def includes(n)
    return <<-EOS
        #include <boost/hana/ext/boost/fusion.hpp>
        #include <boost/fusion/tuple.hpp>
    EOS
end

def list(xs_t, xs)
    return "boost::fusion::make_tuple(#{xs.join(', ')})"
end
%>
