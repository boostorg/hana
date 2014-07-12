<%
def includes(n)
    n = [10, n.round_up(1)].max
    return <<-EOS
        #include <boost/fusion/container/vector/vector#{n}.hpp>
        #include <boost/hana/ext/boost/fusion.hpp>
    EOS
end

def list(xs_t, xs)
  return "boost::fusion::vector#{xs.size}<#{xs_t.join(', ')}>{#{xs.join(',')}}"
end
%>
