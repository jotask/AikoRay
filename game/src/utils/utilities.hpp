#pragma once

#include <string>
#include <fstream>
#include <filesystem>

namespace aiko
{
    namespace utils
    {

        template<class T>
        static T map(T value, T inFrom, T inTo, T outFrom, T outTo)
        {
            return (value - inFrom) * (outTo - outFrom) / (inTo - inFrom) + outFrom;
        }

        static std::filesystem::path getFullPathToFile(const std::string& file)
        {
            // TODO : Not happy with this, find other solution
            return std::filesystem::current_path() / ("../assets/" + file);
        }

        static std::string readFile(const std::string& fileName)
        {

            const auto fullPathToFile = getFullPathToFile(fileName);

            std::string shaderCode;
            std::ifstream file(fullPathToFile, std::ios::in);

            if (file.good() == false)
            {
                printf("Cannot read shader file %s\n", fileName.c_str());
                std::terminate();
            }

            file.seekg(0, std::ios::end);
            shaderCode.resize(static_cast<unsigned int>(file.tellg()));
            file.seekg(0, std::ios::beg);
            file.read(&shaderCode[0], shaderCode.size());
            file.close();
            return shaderCode;
        }

    }

}