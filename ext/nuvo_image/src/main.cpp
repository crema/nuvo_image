#include <iostream>
#include <memory>
#include "ImageProcessor.h"

void PrintJson(const picojson::object & object) {
    auto outputString = picojson::value(object).serialize();
    std::cout << outputString << std::endl;
}

void PrintErrorToJson(const std::string & error) {
    picojson::object result;
    result["error"] = picojson::value(error);
    PrintJson(result);
}

int main(int argc, char *argv[]) {
    std::shared_ptr<ImageProcessor> processor(new ImageProcessor());

    while(true){
        std::string inputString;
        std::getline(std::cin, inputString);

        try {
            auto process = processor->Parse(inputString);
            auto result = process->Process();

            PrintJson(result);
        } catch (const std::runtime_error & e) {
            PrintErrorToJson(e.what());
        } catch (const std::exception & e) {
            PrintErrorToJson(e.what());
        }
    }
    return 0;
}

