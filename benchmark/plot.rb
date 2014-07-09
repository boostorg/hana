require 'benchcc'


time_output, memusg_output, inputs = ARGV
inputs = inputs.split(';')
Benchcc::plot_time(time_output, *inputs)
Benchcc::plot_memusg(memusg_output, *inputs)