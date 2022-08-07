#pragma once

#include <vector>

#include "core/aiko_types.hpp"

namespace aiko
{

    class Module;
    class System;

    using ModulesCollection = std::vector<Scope<Module>>;
    using SystemCollection = std::vector<Scope<System>>;

}
