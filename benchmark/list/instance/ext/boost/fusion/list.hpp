<%
def includes(xs)
    return <<-EOS
        #define FUSION_MAX_LIST_SIZE 50
        #include <boost/fusion/container/list/list.hpp>
        #include <boost/hana/ext/boost/fusion.hpp>
    EOS
end

def list(xs_t, xs)
  return "boost::fusion::list<#{xs_t.join(', ')}>{#{xs.join(',')}}"
end
%>
