<%
def includes(n)
    return <<-EOS
        #include <boost/hana/ext/std/tuple.hpp>
        #include <tuple>
    EOS
end

def list(xs_t, xs)
  return "std::make_tuple(#{xs.join(', ')})"
end
%>
