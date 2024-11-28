/*
#include "Base.h"
#include <iostream>
#include <windows.h>

using namespace flt::refl;
using Dllfunc = std::vector<Type*> (*)();


#include "TestMacro.h"

int main()
{
	constexpr const char* str = CREATE_FILE_LINE;

	constexpr int num = MYNUM;

	/ *HMODULE myDll = LoadLibrary(L"DLLTest.dll");
	Dllfunc func = (Dllfunc)GetProcAddress(myDll, "GetAllTypes");

	std::vector<Type*> types = func();

	Type* baseType = Type::GetType<Base>();
	Type* derivedType = Type::GetType<Derived>();

	std::vector<Property*> baseProps = baseType->GetProperties();
	std::vector<Property*> derivedProps = derivedType->GetProperties();

	std::cout << "Base Properties | ";
	for(auto prop : baseProps)
	{
		std::cout << prop->Name() << " | ";
	}
	std::cout << "\n";

	std::cout << "Derived Properties : | ";
	for(auto prop : derivedProps)
	{
		std::cout << prop->Name() << " | ";
	}
	std::cout << "\n\n";

	std::vector<Method*> baseMethods = baseType->GetMethods();
	std::vector<Method*> derivedMethods = derivedType->GetMethods();
	Base tBase;

	baseMethods[0]->Invoke<void>(&tBase, 1);

	std::cout << "Base Methods | ";
	for(auto method : baseMethods)
	{
		std::cout << method->Name() << " | ";
	}
	std::cout << "\n";

	std::cout << "Derived Methods : | ";
	for(auto method : derivedMethods)
	{
		std::cout << method->Name() << " | ";
	}
	std::cout << "\n\n";

	for (int i = 0; i < 3; ++i)
	{
		std::cout << "derived Class Set Data\n";
		std::cout << "Input Member Name : ";
		std::string memberName;
		std::cin >> memberName;

		Derived derived;
		Property* prop = derivedType->GetProperty(memberName);

		while (!prop)
		{
			std::cout << "Invalid Member Name\n";
			std::cout << "Input Member Name : ";
			std::cin.clear();
			std::cin >> memberName;
			prop = derivedType->GetProperty(memberName);
		}

		{
			std::cout << "input Int Value : ";
			int input;
			std::cin >> input;
			bool ret = prop->Set(&derived, input);
			if (!ret)
			{
				std::cout << "Invalid Value Type\n";
				continue;
			}

			std::cout << "check Derived Member Value\n";

			std::cout << "Member Name : " << memberName << " | Value : " << *prop->Get<int>(&derived) << std::endl;
		}
		{
			std::cout << "input Int Value : ";
			int input;
			std::cin >> input;
			bool ret = prop->Set(&derived, input);
			if (!ret)
			{
				std::cout << "Invalid Value Type\n";
				continue;
			}

			std::cout << "check Derived Member Value\n";

			std::cout << "Member Name : " << memberName << " | Value : " << *prop->Get<int>(&derived) << std::endl;
		}

	}* /

	return 0;
}
*/


#include <iostream>
#include "../Reflection/Reflection.h"
#include "../HeaderTool/example.h"


struct Test
{
	static flt::refl::Type* InitType()
	{
		static flt::refl::Type s_type{ flt::refl::TypeBuilder<Test>{"Test"} };

		{
			static flt::refl::Method method{s_type, &Test::Func, "Func", *(new flt::refl::Callable(&Test::Func)) };
		}

		return &s_type;
	}
public:
	int Func()
	{
		return 10;
	}

	int Func2() const
	{
		return 20;
	}
};

int main()
{
	Example example;

	flt::refl::Type* exampleType = flt::refl::Type::GetType<Example>();

	std::vector<flt::refl::Property*> baseProps = exampleType->GetProperties();
	std::vector<flt::refl::Method*> baseMethods = exampleType->GetMethods();
	
	Test test;
	const Test test2;
	flt::refl::Callable callable(&Test::Func);
	flt::refl::Type* type = flt::refl::Type::GetType<Test>();
	flt::refl::Method method{ *type, &Test::Func, "Func", callable };

	flt::refl::Callable callable2(&Test::Func2);
	flt::refl::Method method2{ *type, &Test::Func2, "Func2", callable2 };

	int val = callable.Invoke(&test);
	int val2 = callable2.Invoke(&test);
	//int val3 = callable.Invoke(&test);

	//val = method.Invoke<int>(&test);
	val2 = method2.Invoke<int>(&test2);

	std::cout << "Base Properties | ";
	return 0;
}
