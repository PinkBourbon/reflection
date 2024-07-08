#pragma once

#include "CommonMacro.h"
#include "Method.h"


#define METHOD_STRUCT_NAME(LINE, METHOD_NAME) Concatenation(RegistMethod, _, LINE, _, METHOD_NAME)
#define METHOD_STRUCT_INSTANCE_NAME(LINE, METHOD_NAME) Concatenation(_registMethod, _, LINE, _, METHOD_NAME)
#define REGIST_METHOD(PROPERTY_NAME) REGIST_METHOD_WITH_LINE(__LINE__, PROPERTY_NAME)

#define REGIST_METHOD_WITH_LINE(LINE, PROPERTY_NAME)\
inline static struct METHOD_STRUCT_NAME(LINE, PROPERTY_NAME)\
{\
	METHOD_STRUCT_NAME(LINE, PROPERTY_NAME)()\
	{\
		static flt::refl::MethodRegister propertyRegister{&PROPERTY_NAME};\
	}\
}METHOD_STRUCT_INSTANCE_NAME(LINE, PROPERTY_NAME);
