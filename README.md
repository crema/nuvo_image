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
sudo apt-get install g++-6
```

**mac**
```
brew install gcc
```

### libjpeg-turbo

libjpeg를 대체하는 고성능 jpeg 라이브러리. opencv 는 기본적으로 libjpeg 를 쓰는데 libjpeg-turbo 는 이보다 5 배 이상 빠르다 

**linux**
```
sudo apt-get install libjpeg-turbo8-dev
```


### mozjpeg
libjpeg-turbo 보다 느리지만 퀄리티는 더 좋다. 성능보다 퀄리티를 원하면 mozjpeg 를 사용하자 

**linux** 
```
wget https://github.com/mozilla/mozjpeg/releases/download/v3.1/mozjpeg-3.1-release-source.tar.gz
tar -xvf mozjpeg-3.1-release-source.tar.gz
cd mozjpeg
./configure
sudo make install
```

### Opencv 3

**linux**

```
sudo add-apt-repository ppa:amarburg/opencv3
sudo apt-get update
sudo apt-get install libopencv3
```
or
```
sudo apt-get install build-essential
wget https://github.com/Itseez/opencv/archive/3.1.0.zip
unzip 3.1.0.zip
cd opencv-3.1.0
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release 
sudo make install
```
실제 서비스 머신에서는 직접 빌드하는것을 권장한다.
nuvo-image 실행에 필요한 컴포넌트만 설치하려면 cmake 실행시 다음 옵션을 사용하자 
```
cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_opencv_perf_core=OFF -DBUILD_opencv_ts=OFF -DBUILD_opencv_videoio=OFF -DBUILD_opencv_calib3d=OFF -DBUILD_opencv_calib3d=OFF -DBUILD_opencv_flann=OFF -DBUILD_opencv_photo=OFF -DBUILD_opencv_hal=OFF -DBUILD_opencv_ml=OFF -DBUILD_opencv_videostab=OFF -DBUILD_opencv_superres=OFF -DBUILD_opencv_stitching=OFF -DBUILD_opencv_calib3d=OFF -DBUILD_opencv_objdetect=OFF -DBUILD_opencv_video=OFF -DBUILD_opencv_photo=OFF -DBUILD_opencv_ml=OFF

```
libjpeg-turbo 를 사용하는지 꼭 확인하자. 사용하지 않을경우 아래 와같이 직접 설정해 주자 
```
cmake .. -DCMAKE_BUILD_TYPE=Release -DWITH_JPEG=ON -DBUILD_JPEG=OFF -DJPEG_INCLUDE_DIR=/usr/include  -DJPEG_LIBRARY=/usr/lib/x86_64-linux-gnu/libjpeg.so
```
mozjpeg 를 사용하려면 아래와 같이 설정해 주자 
```
cmake .. -DCMAKE_BUILD_TYPE=Release -DWITH_JPEG=ON -DBUILD_JPEG=OFF -DJPEG_INCLUDE_DIR=/opt/mozjpeg/include/ -DJPEG_LIBRARY=/opt/mozjpeg/lib64/libjpeg.so
```

**mac**
```
brew install homebrew/science/opencv3
```


## Installation

### Gemfile

```ruby
gem 'nuvo-image', :github => 'crema/nuvo-image'
```

```
bundle install
```

mac 에서 `bundle install`시에 opencv를 제대로 못찾을 경우 `brew link opencv3 --force` 후 다시 시도해볼 것


### Travis
다음의 내용을 추가하자
```
sudo: true
before_script:
  - sudo ln /dev/null /dev/raw1394  
addons:
  apt:
    packages:
      - g++-6
      - cmake
      - cmake-data
      - libopencv-dev
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

- `filename`: 읽어들이 파일
- `auto_orient`: 자동회전 여부. true 일 경우 exif 에서 정보를 읽어 자동회전한다 
- `flatten`: 알파채널이 있는 이미지의 경우 flatten 하게 된다 그 때 사용할 컬러. `:white`, `:black` 

```ruby
NuvoImage.process do |process|
    process.read('test.png')  # => #<struct NuvoImage::Process::ReadResult id="temp-0", width=1500, height=2181, size=1834587>
end
```

###### crop(image, width, height, gravity: :Center)

이미지를 잘라내고 정보를 반환한다 

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

이미지를 리사이징 하고 정보를 반환한다 

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

###### jpeg(image, filename, quality: :high, min: 50, max: 100, search: 3, gray_ssim: true)

이미지를 jpeg 로 압축하여 저장한다 

- `image`: 리사이징 할 이미지. read, crop, resize 의 반환값을 사용가능하다 
- `filename`: 저장할 파일이름 
- `quality`: jpeg 퀄리티 파라메터로 3가지 모드가 있다 
  - `1 ~ 99`: jpeg 퀄리티를 직접 지정한다 
  - `0.0 ~ 1.0`: ssim 값을 직접 지정한다 
  - `:low` : ssim 0.90
  - `:medium`: ssim 0.93
  - `:high`: ssim 0.96
  - `:very_high` - ssim 0.99
- `min`: jpeg 퀄리티 최소값. jpeg 퀄리티가 직접 설정될 경우 무시된다 
- `max`: jpeg 퀄리티 최대값. jpeg 퀄리티가 직접 설정될 경우 무시된다 
- `search`: 퀄리티 검색을 할 경우 최대 검색 횟수. jpeg 퀄리티가 직접 설정될 경우 무시된다
- `gray_ssim`: 퀄리티 검색을 할 경우 ssim 을 얻을 때 grayscale 을 사용할 지 여부, 사용할 경우 검색일 빨라지나 품질이 떨어진다. jpeg 퀄리티가 직접 설정될 경우 무시된다

```ruby
NuvoImage.process do |process|
    image = process.read('test.png')  # => #<struct NuvoImage::Process::ReadResult id="temp-0", width=1500, height=2181, size=1834587>
    process.jpeg(image, 'test.jpg') # => #<struct NuvoImage::Process::JpegResult id="test.jpg", size=128230, quality=50>
end
```
