<%
def includes(n)
    return <<-EOS
        #include <boost/hana/ext/boost/tuple.hpp>
        #include <boost/tuple/tuple.hpp>
    EOS
end

def list(xs_t, xs)
  return "boost::make_tuple(#{xs.join(', ')})"
end
%>
