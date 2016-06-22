# coding: utf-8
lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'nuvo_image/version'

Gem::Specification.new do |spec|
  spec.name          = "nuvo-image"
  spec.version       = NuvoImage::VERSION
  spec.authors       = ["whonz"]
  spec.email         = ["whonzmail@gmail.com"]

  spec.summary       = 'image process library for nuvo'
  spec.description   = 'image process library for nuvo`s crema service'
  spec.homepage      = 'https://github.com/cremame/nuvo-image'
  spec.license       = 'MIT'

  spec.files         = Dir['ext/**/*.cpp', 'ext/**/*.h', 'ext/**/*.rb', 'ext/**/CMakeLists.txt',  'lib/**/*.rb', 'bin/*.rb']
  spec.bindir        = 'bin'
  spec.extensions    = ['ext/nuvo_image_process/extconf.rb']

  spec.add_development_dependency "bundler", "~> 1.12"
  spec.add_development_dependency "rake", "~> 10.0"
  spec.add_development_dependency 'rubocop', '~> 0.35.0'
  spec.add_development_dependency 'minitest'
end
