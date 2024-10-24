﻿#pragma once
#include <iostream>
#include "./generated/example_generated.h"


namespace refl
{
	using func = void;
	using prop = void;

	class InNameSpaceClass
	{};
}

#define REFL_CLASS()
#define REFL_CLASS(X)

#define REFL_FUNC
#define REFL_PROP

#define NOT_AUTO
#define REFL_BODY

template<typename T>
class Tester
{
};


//REFL_CLASS()
REFL_CLASS(NOT_AUTO)
class Example : public Tester<int>, public Tester<float>, virtual public Tester<double>
{
	REFL_BODY
public:
	Example() : _value(3), _valueF(2.5f) {}
	~Example() = default;

	REFL_FUNC
	void Print() const
	{
		//std::cout << "Hello, World!" << std::endl;
		//std::cout << _value << ", " << _valueF << std::endl;
	}

	int GetValue() const
	{
		return _value;
	}

private:
	int _value;
	REFL_PROP
	float _valueF;
	REFL_PROP
	Tester<int> _tester;
};
