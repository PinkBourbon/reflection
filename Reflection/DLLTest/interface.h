#pragma once
#include <vector>
#include "../Reflection/Type.h"


extern "C" __declspec(dllexport) std::vector<flt::refl::Type*> GetAllTypes();
