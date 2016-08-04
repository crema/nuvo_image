# NuvoImage

누보에서 이미지 리사이징을 위해 사용하기 위한 gem 입니다

## Requirement

### cmake

**linux**
```
sudo add-apt-repository ppa:george-edison55/cmake-3.x
sudo apt-get update
sudo apt-get install cmake
```
**mac**
```
brew install cmake
```

### gcc6
**linux**
```
sudo add-apt-repository ppa:ubuntu-toolchain-r/test  
sudo apt-get update  
sudo apt-get install g++-6
```

**mac**
```
brew install gcc
```

### NASM/YASM

**linux**
```
sudo apt-get install nasm yasm
```

**mac**
```
brew install nasm yasm
```

### media libraries

**linux**
```
sudo apt-get install libgtk-3-dev libpng12-dev libtiff4-dev libjasper-dev libwebp-dev libx264-dev libvpx-dev
```

**mac**
```
brew install gtk+ jpeg png tiff jasper webp x264 vpx
```

## Installation

### Gemfile

```ruby
gem 'nuvo-image', :github => 'crema/nuvo-image'
```

```
bundle install
```

### Travis
다음의 내용을 추가하자
```
sudo: true
before_script:
  - sudo ln /dev/null /dev/raw1394  
addons:
  apt:
    packages:
      - build-essential
      - g++-6
      - cmake
      - cmake-data
      - nasm
      - yasm
      - libgtk-3-dev
      - libpng12-dev 
      - libtiff4-dev 
      - libjasper-dev 
      - libwebp-dev 
      - libx264-dev 
      - libvpx-dev
    sources:
      - ubuntu-toolchain-r-test
      - george-edison55-precise-backports
```

## Usage


### Ruby API


module NuvoImage
----------------

##### class methods

###### process

`NuvoImage::Process` 인스턴스를 하나 만들어 yield 하는 메소드 
yield 실행 후에 `NuvoImage::Process` 의 `close` 메소드를 호출한다

``` ruby
NuvoImage.process do |process|
    ...
end
```


class Process
-------------

c++ 로 만들어진 nuvo_image 프로세스를 감싸고 있는 클래스다 
직접 인스턴스를 만들어서 사용할 수 도 있지만 되도록이면 NuvoImage.process 메소드를 사용하자 

##### instance methods

###### read(filename, auto_orient: true, flatten: :white)

이미지 파일을 읽어서 메모리에 저장하고 저장 정보를 반환한다 
gif의 경우 반환 값에 frames 정보가 있다 

- `filename`: 읽어들이 파일
- `auto_orient`: 자동회전 여부. true 일 경우 exif 에서 정보를 읽어 자동회전한다 
- `flatten`: 알파채널이 있는 이미지의 경우 flatten 하게 된다 그 때 사용할 컬러. `:white`, `:black` 

```ruby
NuvoImage.process do |process|
    process.read('test.png')  # => #<struct NuvoImage::Process::ReadResult id="temp-0", width=1500, height=2181, size=1834587, frames=nil>
    process.read('test.gif')  # => #<struct NuvoImage::Process::ReadResult id="temp-1", width=320, height=240, size=3854587, frames=12>
end
```

###### frame(image, frame)

여러 프레임인 이미지에서 특정 프레임을 읽어들인다

- `image`: 프레임을 얻어올 이미지. 반드시 여러 프레임인 이미지여야 한다 
- `frame`: 얻어올 프레임 

```ruby
NuvoImage.process do |process|
    image = process.read('test.gif')  # => #<struct NuvoImage::Process::ReadResult id="temp-1", width=320, height=240, size=3854587, frames=12>
    process.frame(image, 0) # => #<struct NuvoImage::Process::FrameResult id="temp-2", width=320, height=240, frames=nil>
end
```


###### crop(image, width, height, gravity: :Center)

이미지를 잘라내고 정보를 반환한다. gif의 경우 전체 프레임에 적용된다   

- `image`: 잘라 낼 이미지. read, crop, resize 의 반환값을 사용가능하다 
- `width`: 잘라 낼 넓이
- `height`: 잘라 낼 높이
- `gravity`: 잘라 낼 때에 사용할 모드. `:Center`, `:North`, `:South`, `:West`, `:East`, `:NorthWest`, `:NorthEast`, `:SouthWest`, `:SouthEast`

```ruby
NuvoImage.process do |process|
    image = process.read('test.png')  # => #<struct NuvoImage::Process::ReadResult id="temp-0", width=1500, height=2181, size=1834587>
    process.crop(image, 1000, 1000) # => #<struct NuvoImage::Process::CropResult id="temp-1", width=1000, height=1000, gravity=:Center>
end
```

###### resize(image, width, height, interpolation: :area)

이미지를 리사이징 하고 정보를 반환한다. gif의 경우 전체 프레임에 적용된다  

- `image`: 리사이징 할 이미지. read, crop, resize 의 반환값을 사용가능하다 
- `width`: 리사이징 넓이
- `height`: 리사이징 높이
- `interpolation`: 리사이징 할 때 사용할 필터. `:nearest`, `:linear`, `:cubuc`, `:area`, `:lanczos`

```ruby
NuvoImage.process do |process|
    image = process.read('test.png')  # => #<struct NuvoImage::Process::ReadResult id="temp-0", width=1500, height=2181, size=1834587>
    process.crop(image, 1000, 1000) # => #<struct NuvoImage::Process::ResizeResult id="temp-1", width=1000, height=1000, interpolation=:area>
end
```

###### lossy(image, filename, format: :jpeg, quality: :high, min: 50, max: 100, search: 3)

이미지를 jpeg 로 압축하여 저장한다 

- `image`: 저장할 이미지. read, crop, resize 의 반환값을 사용가능하다 
- `filename`: 저장할 파일이름 
- `format`: 저장할 파일 포맷, `:jpeg`, `:webp`
- `quality`: jpeg 퀄리티 파라메터로 3가지 모드가 있다 
  - `1 ~ 99`: jpeg 퀄리티를 직접 지정한다 
  - `0.0 ~ 1.0`: ssim 값을 직접 지정한다 
  - `:low` : ssim 0.90
  - `:medium`: ssim 0.93
  - `:high`: ssim 0.96
  - `:very_high` - ssim 0.99
- `min`: jpeg 퀄리티 최소값. jpeg 퀄리티가 직접 설정될 경우 무시된다 
- `max`: jpeg 퀄리티 최대값. jpeg 퀄리티가 직접 설정될 경우 무시된다 
- `search`: 퀄리티 검색을 할 경우 최대 검색 횟수. 퀄리티가 직접 설정될 경우 무시된다

```ruby
NuvoImage.process do |process|
    image = process.read('test.png')  # => #<struct NuvoImage::Process::ReadResult id="temp-0", width=1500, height=2181, size=1834587, frames=nil>
    process.lossy(image, 'test.jpg', format: :jpeg) # => #<struct NuvoImage::Process::LossyResult id="test.jpg", size=128230, quality=50>
end
```

###### video(image, filename, format: :mp4)

여러 프레임인 이미지를 비디오로 저장한다 

- `image`: 저장할 이미지. 반드시 여러 프레임인 이미지여야 한다 
- `filename`: 저장할 파일이름 
- `format`: 저장할 파일 포맷, `:mp4`, `:webm`

```ruby
NuvoImage.process do |process|
    image = process.read('test.gif')  # => #<struct NuvoImage::Process::ReadResult id="temp-1", width=320, height=240, size=3854587, frames=12>
    process.video(image, 'test.mp4', format: :mp4) # => #<struct NuvoImage::Process::VideoResult id="temp-2", size=2601023 >
end
```