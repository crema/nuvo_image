require 'bundler/gem_tasks'
require 'rake/testtask'

Rake::TestTask.new do |t|
  t.libs.push 'lib'
  t.pattern = 'spec/**/*_test.rb'
  t.verbose = true
end

desc "Run tests"
task :default => :test