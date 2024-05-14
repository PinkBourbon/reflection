#pragma once
#include "Type.h"

#define GENERATED_REFLECT(THIS_TYPE)\
public:\
	template<typename T, typename U>\
	friend struct flt::refl::SuperType;\
\
	template<typename T>\
	friend struct flt::refl::TypeBuilder;\
	\
	using Super = flt::refl::SuperType<THIS_TYPE>::Type;\
	using ThisType = THIS_TYPE;\
\
public:\
	virtual flt::refl::Type* GetType() const\
	{\
		return _type;\
	}\
\
private:\
	static flt::refl::Type* InitType()\
	{\
		static flt::refl::Type s_type{flt::refl::TypeBuilder<THIS_TYPE>{#THIS_TYPE}};\
		return &s_type;\
	}\
	\
	inline static flt::refl::Type* _type = InitType();
