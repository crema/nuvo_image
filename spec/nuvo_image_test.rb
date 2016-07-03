require 'minitest/autorun'
require 'nuvo_image'

describe NuvoImage::Process do
  before do
    Dir.chdir(File.dirname(__FILE__) + '/../ext/nuvo_image/') do
      `cmake .`
      `make`
    end
  end

  subject {NuvoImage::Process.new}

  describe '#read' do
    it 'read' do
      @logo = subject.read(File.dirname(__FILE__) + '/images/sushi.jpg')
      @logo.width.must_equal 960
      @logo.height.must_equal 960
      @logo.size.must_equal 116_306
    end

    after do
      subject.close
    end
  end

  describe '#crop' do
    before do
      @logo = subject.read(File.dirname(__FILE__) + '/images/sushi.jpg')
    end

    it 'crop' do
      [:Center, :North, :South, :East, :West, :NorthEast, :NorthWest, :SouthEast, :SouthWest].each do |gravity|
        cropped = subject.crop(@logo, 100, 50, gravity: gravity)
        cropped.width.must_equal 100
        cropped.height.must_equal 50
        cropped.gravity.must_equal gravity
      end
    end

    after do
      subject.close
    end
  end

  describe '#resize' do
    before do
      @logo = subject.read(File.dirname(__FILE__) + '/images/sushi.jpg')
    end

    it 'resize' do
      [:nearest, :linear, :cubic, :area, :lanczos].each do |interpolation|
        resized = subject.resize(@logo, 100, 50, interpolation: interpolation)
        resized.width.must_equal 100
        resized.height.must_equal 50
        resized.interpolation.must_equal interpolation
      end
    end

    after do
      subject.close
    end
  end

  describe '#jpeg' do
    before do
      @logo = subject.read(File.dirname(__FILE__) + '/images/sushi.jpg')
    end

    it 'jpeg' do
      @low_size = 0
      @low_quality = 0
      [:low, :medium, :high, :very_high].each do |quality|
        jpeg = subject.jpeg(@logo, File.dirname(__FILE__) + "/images/test/#{quality}.jpg", quality: quality)
        assert jpeg.size >= @low_size, 'must less size'
        assert jpeg.quality >= @low_quality, 'must less quality'
        @low_size = jpeg.size
        @low_quality = jpeg.quality
      end
    end

    after do
      subject.close
    end
  end

  describe '#clear' do
    it 'return true' do
      subject.clear.must_equal true
    end

    after do
      subject.close
    end
  end

  describe '#close' do
    it 'return 0' do
      subject.close.must_equal 0
    end
  end
end
