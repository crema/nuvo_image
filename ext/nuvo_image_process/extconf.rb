require 'mkmf'

dir = File.expand_path(File.dirname(__FILE__))
install_dir = "#{dir}/../.."
puts "install dir: #{install_dir}"

command = "cmake ./ -DCMAKE_INSTALL_PREFIX=#{install_dir}"

puts command
puts `#{command}`

$makefile_created = true
