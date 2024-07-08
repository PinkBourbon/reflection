#pragma once
#include <iostream>

namespace flt
{
	namespace refl
	{
		class Method
		{

		};

		template<typename ClassType, typename ReturnType, typename... Args>
		class MethodRegister
		{
		public:
			MethodRegister(ReturnType (ClassType::*ptr)(Args...))
			{
				//std::cout << "MethodRegister" << std::endl;
				//std::cout << typeid(ptr).name() << std::endl;
			}
		};
	} // namespace refl
} // namespace flt
