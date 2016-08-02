require 'json'
require 'open3'

module NuvoImage
  class Process
    attr_reader :stdin, :stdout, :thread

    ReadResult = Struct.new(:id, :width, :height, :size, :frames)
    CropResult = Struct.new(:id, :left, :top, :width, :height, :gravity, :frames)
    ResizeResult = Struct.new(:id, :width, :height, :interpolation, :frames)
    FrameResult = Struct.new(:id, :width, :height, :frame, :frames)
    LossyResult = Struct.new(:id, :size, :quality, :format)
    VideoResult = Struct.new(:id, :size, :format)

    def initialize
      nuvo_image_process = File.join(File.dirname(__FILE__), '/../../ext/nuvo_image/bin/nuvo_image')
      library_path = File.join(File.dirname(__FILE__), '../../ext/nuvo_image/external/lib/opencv3.1.0/lib')

      env = RUBY_PLATFORM.include?('darwin') ? {'DYLD_LIBRARY_PATH'=>library_path} : {'LD_LIBRARY_PATH'=>library_path}
      @stdin, @stdout, @thread = Open3.popen2(env, nuvo_image_process)
    end

    def call(args)
      stdin.puts(args.to_json)
      line = stdout.readline
      result = JSON.parse(line, symbolize_names: true)
      raise result[:error] unless result[:error].nil?
      result
    end

    def read(filename, auto_orient: true, flatten: :white)
      result = call process: :read, from: filename, auto_orient: auto_orient, flatten: flatten
      ReadResult.new(result[:to], result[:width], result[:height], result[:size], result[:frames])
    end

    def crop(image, width, height, gravity: :Center)
      result = call process: :crop, from: image.id, width: width, height: height, gravity: gravity
      CropResult.new(result[:to], result[:left], result[:top], result[:width], result[:height], result[:gravity].to_sym, result[:frames])
    end

    def resize(image, width, height, interpolation: :area)
      result = call process: :resize, from: image.id, width: width, height: height, interpolation: interpolation
      ResizeResult.new(result[:to], result[:width], result[:height], result[:interpolation].to_sym, result[:frames])
    end

    def lossy(image, filename, format: :jpeg, quality: :high, min: 50, max: 100, search: 3)
      result = call process: :lossy, from: image.id, to: filename, format: format, quality: quality, min: min, max: max, search: search
      LossyResult.new(result[:to], result[:size], result[:quality], result[:format])
    end

    def frame(image, frame)
      result = call process: :frame, from: image.id, frame: frame
      FrameResult.new(result[:to], result[:width], result[:height], result[:frame], result[:frames])
    end

    def video(image, filename, format: :mp4)
      result = call process: :video, from: image.id, to: filename, format: format
      VideoResult.new(result[:to], result[:size], result[:format])
    end

    def clear
      result = call process: :clear
      result[:result]
    end

    def close
      stdin.puts({process: :close}.to_json)
      thread.value
    end
  end
end
