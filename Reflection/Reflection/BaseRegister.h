#pragma once
#include "Type.h"

namespace flt
{
	namespace refl
	{
		class BaseRegister
		{
			friend class flt::refl::Type;
		public:
			BaseRegister(Type* type, Type* baseType)
			{
				type->AddParents(baseType);
			}
		};
	} // namespace refl
} // namespace flt
