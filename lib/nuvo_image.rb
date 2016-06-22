require 'nuvo_image/version'
require 'nuvo_image/process'
require 'open3'


module NuvoImage
  def self.Process(&_block)
    process = Process.new

    yield process

    process.close
  end
end
