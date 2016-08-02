require 'mkmf'

dir = File.expand_path(File.dirname(__FILE__))

puts `cmake .. -DCMAKE_INSTALL_PREFIX=#{File.join(dir, '../')}`
$makefile_created = true
