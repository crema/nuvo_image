require 'mkmf'

dir = File.expand_path(File.dirname(__FILE__))
command = "cmake ./ "

puts command
puts `#{command}`

$makefile_created = true
