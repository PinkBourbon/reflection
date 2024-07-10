#include "Type.h"
#include "Property.h"
#include "Method.h"

flt::refl::Property* flt::refl::Type::GetProperty(std::string_view name) const
{
	for (auto& property : _properties)
	{
		if (property->name == name)
		{
			return property;
		}
	}

	return nullptr;
}

std::vector<flt::refl::Property*> flt::refl::Type::GetProperties() const
{
	return _properties;
}

flt::refl::Method* flt::refl::Type::GetMethod(std::string_view name) const
{
	for(auto& method : _methods)
	{
		if (method->_name == name)
		{
			return method;
		}
	}
}

std::vector<flt::refl::Method*> flt::refl::Type::GetMethods() const
{
	return _methods;
}
