#pragma once
#include "../Reflection/Reflection.h"


class Base
{
	GENERATED_REFLECT(Base)

public:

	REGIST_PROPERTY(baseInt)
	int baseInt = 10;

	REGIST_METHOD(Func)
	void Func(int i);

private:
	REGIST_PROPERTY(_baseFloat)
	float _baseFloat = 10.1f;
};

class Derived : public Base
{
	GENERATED_REFLECT(Derived)

public:
	REGIST_PROPERTY(derivedInt)
	int derivedInt = 20;

	float derivedFloat = 20.2f;
private:

	REGIST_PROPERTY(_derivedInt)
	int _derivedInt = 22;
};


/// 목표 코드 모양
/*
class Base
{
	GENERATED_REFLECT

public:

	REGIST_PROPERTY
	int baseInt = 10;

	REGIST_METHOD
	void Func(int i);

private:
	REGIST_PROPERTY
	float _baseFloat = 10.1f;
};
*/

/// 생성 되어야 하는 코드 모양
/*
#undef GENERATED_REFLECT\
#define GENERATED_REFLECT\
public:\
	virtual flt::refl::Type* GetType() const\
	{\
		return _type;\
	}\
	static flt::refl::Type* InitType()\
	{\
		static flt::refl::Type s_type{flt::refl::TypeBuilder<Base>{"Base"}};\
		\
		{static Property property(&s_type, baseInt, Type::GetType<decltype(baseInt)>(), new PropertyHandler(&Base::baseInt));}\
		{static Method method(&s_type, &Base::Func, "Func", new Callable<Base, void, int>(&Base::Func));}\
		{static Property property(&s_type, _baseFloat, Type::GetType<decltype(_baseFloat)>(), new PropertyHandler(&Base::_baseFloat));}\
		return &s_type;\
	}\
\
private:\
	\
	inline static flt::refl::Type* _type = InitType();

*/
