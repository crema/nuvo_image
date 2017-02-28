#include <fstream>
#include <memory>
#include "ImageProcess.h"
#include "ImageProcessor.h"

const ImageProcessInput ImageProcess::ReadFrom(const std::string& fromString, const int fromType) {
  ImageProcessInput input;

  if (fromType == ProcessInputType::File) {
    std::ifstream file(fromString, std::ios::in | std::ios::binary);

    if (!file.good()) {
      throw std::runtime_error("can not open file: " + fromString);
    }
    file.seekg(0, std::ios::end);
    auto size = (int)file.tellg();
    file.seekg(0, std::ios::beg);

    std::shared_ptr<std::vector<unsigned char>> buffer(new std::vector<unsigned char>(size));
    file.read((char*)buffer->data(), size);

    input = ImageProcessInput(buffer);
  } else if (fromType != ProcessInputType::InvalidInput) {
    if (!processor->TryGet(fromString, input)) {
      throw std::runtime_error("key not exist: " + fromString);
    }
  }

  if (!(input.GetType() & fromType)) {
    throw std::runtime_error("input type missmatch");
  }
  return input;
}

void ImageProcess::WriteTo(const std::string& toString, const int toType, const ImageProcessInput& output) {
  if (toString.length() == 0) {
    throw std::runtime_error(GetName() + " must have to");
  }

  if (toType == ProcessInputType::File) {
    auto buffer = output.GetBuffer();

    if (buffer->empty()) {
      throw std::runtime_error("empty output");
    }

    if (!output.IsWritten()) {
      std::ofstream file(toString, std::ios::out | std::ofstream::binary);
      file.write((const char*)buffer->data(), buffer->size());
    }

  } else if (toType != ProcessInputType::InvalidInput) {
    if (output.Empty()) {
      throw std::runtime_error("empty output");
    }

    if (!(output.GetType() & toType)) {
      throw std::runtime_error("output type missmatch");
    }
    processor->Insert(toString, output);
  }
}

picojson::object InOutProcess::Process() {
  picojson::object result;

  result["process"] = picojson::value(GetName());

  result["from"] = picojson::value(from);
  auto input = ReadFrom(from, GetFromType());

  if (input.GetType() == ProcessInputType::File) {
    result["size"] = picojson::value((double)input.GetBuffer()->size());
  }

  auto output = Process(input, result);

  if (to.length() == 0 && GetToType() != ProcessInputType::File) {
    this->to = processor->GetTempName();
  }

  result["to"] = picojson::value(to);
  WriteTo(to, GetToType(), output);

  if (output.GetType() == ProcessInputType::File) {
    result["size"] = picojson::value((double)output.GetBuffer()->size());
  }
  return result;
}

picojson::object TwoImageToNullProcess::Process() {
  picojson::object result;
  result["process"] = picojson::value(GetName());

  result["from1"] = picojson::value(from1);
  result["from2"] = picojson::value(from2);

  auto input1 = ReadFrom(from1, ProcessInputType::Image | ProcessInputType::Animation);
  auto input2 = ReadFrom(from2, ProcessInputType::Image | ProcessInputType::Animation);
  Process(input1, input2, result);

  return result;
}

picojson::object NullToNullProcess::Process() {
  picojson::object result;
  result["process"] = picojson::value(GetName());
  Process(result);
  return result;
}