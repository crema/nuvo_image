require 'active_support/concern'

module NuvoImage
  module CarrierWave
    extend ActiveSupport::Concern
  end

  module ClassMethods
    def read(to, auto_orient=true)
      process read: [to, auto_orient]
    end

    def crop(from, to, width, height, gravity=:Center)
      process crop: [from, to, auto_orient]
    end

    def resize(from, to, width, height, interpolation=:area)
      process resize: [from, to, width, height, interpolation]
    end

    def jpeg(from, quality=:high)
      process jpeg: [from, quality]
    end
  end

  attr_reader :nuvo_image, :nuvo_images

  def cache!(new_file)
    NuvoImage.process do |nuvo_image|
      @nuvo_images = {}
      @nuvo_image = nuvo_image
      super
      @nuvo_image = nil
      @nuvo_images = {}
    end
  end

  def read(from, to, auto_orient)
    nuvo_images[to] = nuvo_image.read(file.path, auto_orient)
  end

  def crop(from, to, width, height, gravity)
    nuvo_images[to] = nuvo_image.crop(nuvo_images[from], width, height, gravity)
  end

  def resize(from, to, width, height, interpolation)
    nuvo_images[to] = nuvo_image.resize(nuvo_images[from], width, height, interpolation)
  end

  def jpeg(from, to, quality)
    nuvo_image.jpeg(nuvo_images[from], file.path, quality)
  end
end