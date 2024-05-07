#include <iostream>
#include <type_traits>


template<class R, class... Args>
struct function_traits
{
	using return_type = R;
	static constexpr std::size_t arity = sizeof...(Args);

	void PrintType()
	{
		std::cout << "Return type: " << typeid(R).name() << std::endl;
		std::cout << "argument : " << typeid(Args).name << ... << std::endl;
	}
};

int main()
{

}
