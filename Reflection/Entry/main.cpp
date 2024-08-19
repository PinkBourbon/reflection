#include "Base.h"
#include <iostream>
#include <windows.h>

using namespace flt::refl;
using Dllfunc = std::vector<Type*> (*)();


int main()
{
	HMODULE myDll = LoadLibrary(L"DLLTest.dll");
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

	}
	
	return 0;
}
