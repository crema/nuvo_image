#include <fstream>
#include <memory>
#include "ImageProcess.h"
#include "ImageProcessor.h"

picojson::object ImageProcess::Process() {
    picojson::object result;

    result["process"] = picojson::value(GetName());

    auto input = ReadFrom();
    if(HasFrom()) {
        result["from"] = picojson::value(from);
    }
    if(input.GetType() == ProcessInputType::File){
        result["size"] = picojson::value((double)input.GetBuffer().size());
    }
    auto output = Process(input, result);

    WriteTo(output);
    if (HasTo()) {
        result["to"] = picojson::value(to);
    }
    if(output.GetType() == ProcessInputType::File){
        result["size"] = picojson::value((double)output.GetBuffer().size());
    }
    return result;
}

const ImageProcessInput ImageProcess::ReadFrom() {
    if(RequireFrom() && !HasFrom()) {
        throw std::runtime_error(GetName() + " must have from");
    }

    ImageProcessInput input;

    if(GetFromType() == ProcessInputType::File){
        std::ifstream file(from, std::ios::in | std::ios::binary);

        if(!file.good()) {
            throw std::runtime_error("can not open file: " + from);
        }
        file.seekg(0, std::ios::end);
        auto size = (int)file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<unsigned char> buffer(size);
        file.read((char*) buffer.data(), size);

        input = ImageProcessInput(std::move(buffer));
    } else if (GetFromType() == ProcessInputType::Memory) {
        cv::Mat mat;
        if(!processor->TryGet(from, mat)){
            throw std::runtime_error("key not exist: " + from);
        }

        input = ImageProcessInput(mat);
    }

    return input;
}

void ImageProcess::WriteTo(const ImageProcessInput &input) {
    if(RequireTo() && !HasTo()) {
        if(GetToType() == ProcessInputType::Memory){
            to = processor->GetTempName();
        } else {
            throw std::runtime_error(GetName() + " must have to");
        }
    }

    if(GetToType() == ProcessInputType::File){
        auto buffer = input.GetBuffer();

        if(buffer.empty()){
            throw std::runtime_error("empty output");
        }

        std::ofstream file(to, std::ios::out | std::ofstream::binary);
        file.write((const char*)buffer.data(), buffer.size());

    } else if (GetToType() == ProcessInputType::Memory){
        auto mat = input.GetMat();

        if(mat.empty()){
            throw std::runtime_error("empty output");
        }

        processor->Insert(to, mat);
    }
}