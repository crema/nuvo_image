require 'minitest/autorun'
require 'nuvo_image'

describe NuvoImage::Process do
  before do
    Dir.chdir(File.join(File.dirname(__FILE__), '../ext/nuvo_image/build')) do
      `ruby extconf.rb`
      `make install`
    end
  end

  subject {NuvoImage::Process.new}

  describe '#read' do
    it 'should work' do
      @sushi = subject.read(File.dirname(__FILE__) + '/images/sushi.jpg')
      @sushi.width.must_equal 960
      @sushi.height.must_equal 960
      @sushi.size.must_equal 116_306
      @sushi.frames.must_be_nil

      @todd = subject.read(File.dirname(__FILE__) + '/images/todd.gif')
      @todd.frames.must_equal 21
      @todd.width.must_equal 320
      @todd.height.must_equal 240
      @todd.size.must_equal 472_973
    end

    after do
      subject.close
    end
  end

  describe '#crop' do
    before do
      @sushi = subject.read(File.dirname(__FILE__) + '/images/sushi.jpg')
      @todd = subject.read(File.dirname(__FILE__) + '/images/todd.gif')
    end

    it 'should work' do
      {
        Center: [430, 455, 100, 50],
        North: [430, 0, 100, 50],
        South: [430, 910, 100, 50],
        East: [860, 455, 100, 50],
        West: [0, 455, 100, 50],
        NorthEast: [860, 0, 100, 50],
        NorthWest: [0, 0, 100, 50],
        SouthEast: [860, 910, 100, 50],
        SouthWest: [0, 910, 100, 50]
      }.each do |gravity, result|
        cropped = subject.crop(@sushi, 100, 50, gravity: gravity)
        cropped.gravity.must_equal gravity
        [cropped.left, cropped.top, cropped.width, cropped.height].must_equal result
        cropped.frames.must_be_nil
      end

      {
        Center: [110, 95, 100, 50],
        North: [110, 0, 100, 50],
        South: [110, 190, 100, 50],
        East: [220, 95, 100, 50],
        West: [0, 95, 100, 50],
        NorthEast: [220, 0, 100, 50],
        NorthWest: [0, 0, 100, 50],
        SouthEast: [220, 190, 100, 50],
        SouthWest: [0, 190, 100, 50]
      }.each do |gravity, result|
        cropped = subject.crop(@todd, 100, 50, gravity: gravity)
        cropped.gravity.must_equal gravity
        [cropped.left, cropped.top, cropped.width, cropped.height].must_equal result
        cropped.frames.must_equal 21
      end
    end

    after do
      subject.close
    end
  end

  describe '#resize' do
    before do
      @sushi = subject.read(File.dirname(__FILE__) + '/images/sushi.jpg')
      @todd = subject.read(File.dirname(__FILE__) + '/images/todd.gif')
    end

    it 'should work' do
      [:nearest, :linear, :cubic, :area, :lanczos].each do |interpolation|
        resized = subject.resize(@sushi, 100, 50, interpolation: interpolation)
        resized.width.must_equal 100
        resized.height.must_equal 50
        resized.interpolation.must_equal interpolation
        resized.frames.must_be_nil
      end

      [:nearest, :linear, :cubic, :area, :lanczos].each do |interpolation|
        resized = subject.resize(@todd, 100, 50, interpolation: interpolation)
        resized.width.must_equal 100
        resized.height.must_equal 50
        resized.interpolation.must_equal interpolation
        resized.frames.must_equal 21
      end
    end

    after do
      subject.close
    end
  end

  describe '#frame' do
    before do
      @todd = subject.read(File.dirname(__FILE__) + '/images/todd.gif')
    end

    it 'should work' do
      (0..@todd.frames - 1).each do |frame|
        framed = subject.frame(@todd, frame)
        framed.frame.must_equal frame
        framed.width.must_equal 320
        framed.height.must_equal 240
        framed.frames.must_be_nil
      end
    end

    after do
      subject.close
    end
  end

  describe '#lossy' do
    before do
      @sushi = subject.read(File.dirname(__FILE__) + '/images/sushi.jpg')
    end

    it 'should work' do
      jpeg_size = 0
      jpeg_quality = 0
      webp_size = 0
      webp_quality = 0
      [:low, :medium, :high, :very_high].each do |quality|
        jpeg = subject.lossy(@sushi, File.dirname(__FILE__) + '/images/test/#{quality}.jpg', format: :jpeg, quality: quality)
        webp = subject.lossy(@sushi, File.dirname(__FILE__) + '/images/test/#{quality}.webp', format: :webp, quality: quality)
        assert jpeg.size >= jpeg_size, 'must less size'
        assert jpeg.quality >= jpeg_quality, 'must less quality'
        assert webp.size >= webp_size, 'must less size'
        assert webp.quality >= webp_quality, 'must less quality'

        jpeg_size = jpeg.size
        jpeg_quality = jpeg.quality
        webp_size = webp.size
        webp_quality = webp.quality
      end
    end

    after do
      subject.close
    end
  end

  describe '#video' do
    before do
      @todd = subject.read(File.dirname(__FILE__) + '/images/todd.gif')
    end

    it 'should work' do
      mp4 = subject.video(@todd, File.dirname(__FILE__) + '/images/test/todd.mp4', format: :mp4)
      wemb = subject.video(@todd, File.dirname(__FILE__) + '/images/test/todd.webm', format: :webm)
      assert mp4.size > 0, 'must greater than 0'
      assert wemb.size > 0, 'must greater than 0'
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
