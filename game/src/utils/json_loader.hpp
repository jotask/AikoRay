#pragma once

#include "utils/utilities.hpp"

#include <json/json.h>

#include <string>
#include <iostream>

namespace aiko
{
    namespace jsonLoader
    {

        static Json::Value loadJson(const std::string& file)
        {

            auto err = JSONCPP_STRING();
            auto root = Json::Value();

            const auto content = utils::readFile(file);
            const auto jsonLength = static_cast<int>(content.length());

            const auto builder = Json::CharReaderBuilder();
            const auto reader = std::unique_ptr<Json::CharReader>(builder.newCharReader());
            if (reader->parse(content.c_str(), content.c_str() + jsonLength, &root, &err) == false)
            {
                std::cout << "Error reading Json file [" << file << "] with error: " << err << std::endl;
            }

            return std::move(root);

        }

    }
}
