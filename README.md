# NuvoImage

누보에서 이미지 리사이징을 위해 사용하기 위한 gem 입니다

## Requirement

### cmake

linux
```
sudo add-apt-repository ppa:george-edison55/cmake-3.x
sudo apt-get update
sudo apt-get install cmake
```
mac
```
brew install cmake
```

### gcc6
linux
```
sudo apt-get install g++-6
```

mac
```
brew install gcc
```

### libjpeg-turbo

libjpeg를 대체하는 고성능 jpeg 라이브러리. opencv 는 기본적으로 libjpeg 를 쓰는데 libjpeg-turbo 는 이보다 5 배 이상 빠르다 

linux
```
libjpeg-turbo8-dev
```


### mozjpeg
libjpeg-turbo 보다 느리지만 퀄리티는 더 좋다. 성능보다 퀄리티를 원하면 mozjpeg 를 사용하자 

linux 
```
wget https://github.com/mozilla/mozjpeg/releases/download/v3.1/mozjpeg-3.1-release-source.tar.gz
tar -xvf mozjpeg-3.1-release-source.tar.gz
cd mozjpeg
./configure
sudo make install
```

### Opencv 3

linux

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
cmake 실행시 libjpeg-turbo 를 사용하는지 꼭 확인하자. 사용하지 않을경우 아래 와같이 직접 설정해 주자 
```
cmake .. -DWITH_JPEG=ON -DBUILD_JPEG=OFF -DJPEG_INCLUDE_DIR=/usr/include  -DJPEG_LIBRARY=/usr/lib/x86_64-linux-gnu/libjpeg.so
```

mac
```
brew install homebrew/science/opencv3
```


## Installation

Gemfile:

```ruby
gem 'nuvo-image', :github => 'crema/nuvo-image'
```

```
bundle install
```

mac 에서 `bundle install` 시 빌드 제대로 안 되면 `brew link opencv3 --force` 후 다시 시도해볼 것

## Usage

```ruby
require 'nuvo_image'

NuvoImage.Process do |process|
	read = process.read('test.jpeg')
    cropped = process.crop(read, 100, 100, :center)
    resized = process.resize(cropped, 50, 50, :area)
    process.jpeg(resized, 'result.jpg', :high)
end

```

## With Travis

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
