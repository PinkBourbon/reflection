#pragma once
#include <iostream>
#include "../Reflection/Reflection.h"
#include "./generated/example_generated.h"

namespace refl
{
	using func = void;
	using prop = void;

	REFL_CLASS()
		class InNameSpaceClass
	{
		REFL_BODY
	};
}

//REFL_CLASS()
template<typename T>
class Tester
{
	//REFL_BODY
};


//REFL_CLASS()
REFL_CLASS(NOT_AUTO)
class Example : public Tester<int>, public Tester<float>, virtual public Tester<double>
{
	//REFL_BODY

	template<typename T>
	friend struct flt::refl::TypeBuilder;
public:
	virtual flt::refl::Type* GetType() const
	{
		return _type;
	}

	static flt::refl::Type* InitType()
	{
		static flt::refl::Type s_type{ flt::refl::TypeBuilder<Example>{"Example"} };

		{ static flt::refl::BaseRegister baseRegister{ &s_type, flt::refl::Type::GetType<Tester<int>>() }; }
		{ static flt::refl::BaseRegister baseRegister{ &s_type, flt::refl::Type::GetType<Tester<float>>() }; }
		{ static flt::refl::BaseRegister baseRegister{ &s_type, flt::refl::Type::GetType<Tester<double>>() }; }
		{ static flt::refl::Method method{ s_type, &Example::Print, "Print", new flt::refl::Callable(&Example::Print) }; }

		{ static flt::refl::Property property{ &s_type, {"_valueF", flt::refl::Type::GetType<decltype(_valueF)>(), new flt::refl::PropertyHandler(&Example::_valueF)} }; }
		{ static flt::refl::Property property{ &s_type, {"_tester", flt::refl::Type::GetType<decltype(_tester)>(), new flt::refl::PropertyHandler(&Example::_tester)} }; }

		return &s_type;
	}

private:
	inline static flt::refl::Type* _type = InitType();
public:
	Example() : _value(3), _valueF(2.5f) {}
	~Example() = default;

	REFL_FUNC
		void Print() const
	{
		//std::cout << "Hello, World!" << std::endl;
		//std::cout << _value << ", " << _valueF << std::endl;
	}

	REFL_FUNC
	int GetValue() const
	{
		return _value;
	}

	REFL_FUNC
	void SetValue(int value)
	{
		_value = value;
	}

private:
	int _value;
	REFL_PROP
		float _valueF;
	REFL_PROP
		Tester<int> _tester;
};
