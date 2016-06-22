# NuvoImage

누보에서 이미지 리사이징을 위해 사용하기 위한 gem 입니다

## Requirement

### cmake

linux
```
sudo apt-get install cmake
```
mac
```
brew install cmake
```


### Opencv 3.1

linux

```
sudo apt-get install build-essential
sudo apt-get install git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
wget https://github.com/Itseez/opencv/archive/3.1.0.zip
unzip 3.1.0.zip
cd opencv-3.1.0
mkdir build
cd build
cmake -D CMAKE_BUILD_TYPE=Release ./..
make
sudo make install
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

## Usage

```ruby
require 'nuvo_image'

NuvoImage.Process do |process|
	test = process.read('test.jpeg')
    cropped = process.test(test, 100, 100, :center)
    resized = process.resize(cropped, 50, 50, :area)
    process.jpeg(resized, 'result.jpg', :high)
end

```
