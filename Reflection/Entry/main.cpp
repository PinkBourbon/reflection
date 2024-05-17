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

#define STR(A) #A
#define STR2(A) STR(A)
#define TEST_STR test_str

constexpr auto g_t = STR(__LINE__);
constexpr auto g_t2 = STR2(__LINE__);
constexpr auto g_t3 = STR(TEST_STR);
constexpr auto g_t4 = STR2(TEST_STR);

#include <iostream>
#include "ReflTest.h"

int main()
{
	ReflTest test;
	auto type = test.GetType();

	std::cout << std::boolalpha;

	constexpr flt::refl::Tester<ReflTest> t;

	constexpr auto m = t.hasSuper;

	std::cout << m << std::endl;

	std::cout << flt::refl::ReflectType<ReflTest> << std::endl;
	std::cout << flt::refl::ReflectType<Parent> << std::endl;
	std::cout << flt::refl::ReflectType<ReflMember> << std::endl;
	std::cout << flt::refl::ReflectType<float> << std::endl;

	auto type2 = flt::refl::Type::GetType<ReflTest>();
	auto type3 = flt::refl::Type::GetType<Parent>();
	auto type4 = flt::refl::Type::GetType<ReflMember>();
	auto type5 = flt::refl::Type::GetType<float>();


	//static_assert(std::is_same_v<ReflTest, ReflTest::ThisType>);
	//static_assert(std::is_same_v<Parent, ReflTest::Super>);

	std::cout << "type    : " << type->_typeName << std::endl;
	std::cout << "super   : " << type->_super->_typeName << std::endl;
	std::cout << "rawType : " << type->_rawTypeName << std::endl;
	std::cout << "hash    : " << type->_hash << std::endl;

	return 0;
}
