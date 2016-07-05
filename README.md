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

NuvoImage::Process 인스턴스를 하나 만들어 yield 하는 메소드 
yield 실행 후에 NuvoImage::Process 의 close 메소드를 호출한다

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

- filename: 읽어들이 파일
- auto_orient: 
- flatten: 알파채널이 있는 이미지의 경우 flatten 하게 된다 그 때 사용할 컬러, 현재 :white 와 :black 만 지원 

```ruby

NuvoImage.process do |process|
    process.read('test.jpg')  # => 
end
```

###### crop

이미지를 잘라내고 정보를 반환한다 

###### resize

이미지를 리사이징 하고 정보를 반환한다 

###### jpeg

이미지를 jpeg 로 압축하여 저장한다 

###### close
c++ 프로세스를 닫기 위한 메소드 


