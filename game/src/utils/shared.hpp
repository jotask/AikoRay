#pragma once

#include <memory>

namespace aiko
{

    template <class T>
    using AikoPtr = std::shared_ptr<T>;

    template <class T>
    using AikoUPtr = std::unique_ptr<T>;

}
