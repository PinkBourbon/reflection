#define XXH_INLINE_ALL

#include <iostream>
#include <vector>
#include "typename.h"


int main()
{
	std::cout << TypeName<int>::value << std::endl;
	std::cout << TypeName<int>::hash << std::endl;

	constexpr auto name = TypeName_v<int>;
	std::cout << name << std::endl;
	std::cout << TypeName<int>::hash << std::endl;
	static_assert(name == "int", "bad type name");

	static_assert(WTypeName_v<int> == L"int", "bad type name");
	std::wcout << WTypeName_v<int> << std::endl;

	TypeName<std::string> string_name;
	std::cout << string_name.value << std::endl;
	std::cout << string_name.hash << std::endl;

	TypeName<std::wstring> wstring_name;
	std::cout << wstring_name.value << std::endl;
	std::cout << wstring_name.hash << std::endl;

	std::cout << TypeName_v<std::vector<std::wstring>> << std::endl;


	return 0;
}
