#pragma once

#include "CommonMacro.h"
#include "Property.h"

#define PROPERTY_STRUCT_NAME(LINE, PROPERTY_NAME) Concatenation(RegistProperty, _, LINE, _, PROPERTY_NAME)
#define PROPERTY_STRUCT_INSTANCE_NAME(LINE, PROPERTY_NAME) Concatenation(_registProperty, _, LINE, _, PROPERTY_NAME)
#define REGIST_PROPERTY(PROPERTY_NAME) REGIST_PROPERTY_WITH_LINE(__LINE__, PROPERTY_NAME)

#define REGIST_PROPERTY_WITH_LINE(LINE, PROPERTY_NAME)\
inline static struct PROPERTY_STRUCT_NAME(LINE, PROPERTY_NAME)\
{\
	PROPERTY_STRUCT_NAME(LINE, PROPERTY_NAME)()\
	{\
		static flt::refl::PropertyRegister<ThisType, decltype(PROPERTY_NAME), decltype(&ThisType::PROPERTY_NAME), &ThisType::PROPERTY_NAME> propertyRegister{#PROPERTY_NAME, InitType()};\
	}\
}PROPERTY_STRUCT_INSTANCE_NAME(LINE, PROPERTY_NAME);
