#include "Base.h"
#include <iostream>


using namespace flt::refl;

int main()
{
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
