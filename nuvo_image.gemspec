lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'nuvo_image/version'

Gem::Specification.new do |spec|
  spec.name          = 'nuvo_image'
  spec.version       = NuvoImage::VERSION
  spec.authors       = ['whonz']
  spec.email         = ['whonzmail@gmail.com']

  spec.summary       = 'image process library for nuvo'
  spec.description   = 'image process library for nuvo`s crema service'
  spec.homepage      = 'https://github.com/crema/nuvo_image'
  spec.license       = 'MIT'

  spec.files         = Dir['ext/**/*.cpp', 'ext/**/*.h', 'ext/**/*.rb', 'ext/**/*.cmake', 'ext/**/CMakeLists.txt', 'lib/**/*.rb']
  spec.bindir        = 'bin'
  spec.extensions    = ['ext/nuvo_image/build/extconf.rb']

  spec.add_development_dependency 'bundler'
  spec.add_development_dependency 'byebug'
  spec.add_development_dependency 'minitest-ci'
  spec.add_development_dependency 'minitest'
  spec.add_development_dependency 'rake'
  spec.add_development_dependency 'rubocop'
end
