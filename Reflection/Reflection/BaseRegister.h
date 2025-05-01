#pragma once
#include "Type.h"

namespace flt
{
	namespace refl
	{
		/// <summary>
		/// 리플렉션 대상 클래스의 부모 클래스를 등록하기 위한 헬퍼 클래스
		/// </summary>
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
