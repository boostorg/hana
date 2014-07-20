<%
def includes(n)
    puts "n: #{n}, roundedup: #{n.round_up(1)}"
    n = [10, n.round_up(1)].max
    return <<-EOS
        #include <boost/fusion/container/vector/vector#{n}.hpp>
        #include <boost/hana/ext/boost/fusion.hpp>
    EOS
end

def list(xs_t, xs)
  puts "xs_t: #{xs_t}, xs: #{xs}, size: #{xs.size}"
  return "boost::fusion::vector#{xs.size}<#{xs_t.join(', ')}>{#{xs.join(',')}}"
end
%>
