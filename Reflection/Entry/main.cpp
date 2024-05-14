//#define XXH_INLINE_ALL
//
//#include <iostream>
//#include <vector>
//#include "typename.h"
//
//
//int main()
//{
//	std::cout << TypeName<int>::value << std::endl;
//	std::cout << TypeName<int>::hash << std::endl;
//
//	constexpr auto name = TypeName_v<int>;
//	std::cout << name << std::endl;
//	std::cout << TypeName<int>::hash << std::endl;
//	static_assert(name == "int", "bad type name");
//
//	static_assert(WTypeName_v<int> == L"int", "bad type name");
//	std::wcout << WTypeName_v<int> << std::endl;
//
//	TypeName<std::string> string_name;
//	std::cout << string_name.value << std::endl;
//	std::cout << string_name.hash << std::endl;
//
//	TypeName<std::wstring> wstring_name;
//	std::cout << wstring_name.value << std::endl;
//	std::cout << wstring_name.hash << std::endl;
//
//	std::cout << TypeName_v<std::vector<std::wstring>> << std::endl;
//
//
//	return 0;
//}

#include <iostream>
#include "ReflTest.h"

int main()
{
	ReflTest test;
	auto type = test.GetType();

	//static_assert(std::is_same_v<ReflTest, ReflTest::ThisType>);
	//static_assert(std::is_same_v<Parent, ReflTest::Super>);

	std::cout << "type    : " << type->_typeName << std::endl;
	std::cout << "super   : " << type->_super->_typeName << std::endl;
	std::cout << "rawType : " << type->_rawTypeName << std::endl;
	std::cout << "hash    : " << type->_hash << std::endl;

	return 0;
}
