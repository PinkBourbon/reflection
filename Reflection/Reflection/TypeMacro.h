#pragma once
#include "Type.h"

#define GENERATED_REFLECT(THIS_TYPE)\
	template<typename T, typename U>\
	friend struct flt::refl::SuperType;\
\
	template<typename T>\
	friend struct flt::refl::TypeBuilder;\
	friend struct flt::refl::Tester<THIS_TYPE>;\
	\
public:\
	using Super = flt::refl::SuperType<THIS_TYPE>::Type;\
	using ThisType = THIS_TYPE;\
\
	virtual flt::refl::Type* GetType() const\
	{\
		return _type;\
	}\
	static flt::refl::Type* InitType()\
	{\
		static flt::refl::Type s_type{flt::refl::TypeBuilder<THIS_TYPE>{#THIS_TYPE}};\
		return &s_type;\
	}\
\
private:\
	\
	inline static flt::refl::Type* _type = InitType();
