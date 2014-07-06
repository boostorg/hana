require 'benchcc'


time_output, memusg_output, *inputs = ARGV
Benchcc::plot_time(time_output, *inputs)
Benchcc::plot_memusg(memusg_output, *inputs)