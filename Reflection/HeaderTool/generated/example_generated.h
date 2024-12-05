///Generated Header Tool 0.1.0v

#undef CURRENT_FILE_PATH
#define CURRENT_FILE_PATH FTL_FILE_PATH_HEADERTOOL_EXAMPLE_H

#define FTL_FILE_PATH_HEADERTOOL_EXAMPLE_H_30_FLT_REFL\
	template<typename T>\
	friend struct flt::refl::TypeBuilder;\
public:\
	virtual flt::refl::Type* GetType() const\
	{\
		return _type;\
	}\
\
	static flt::refl::Type* InitType()\
	{\
		static flt::refl::Type s_type{flt::refl::TypeBuilder<Example>{"Example"}};\
	\
		{ static flt::refl::BaseRegister baseRegister{ &s_type,, flt::refl::Type::GetType<Tester<int>>()};}\
		{ static flt::refl::BaseRegister baseRegister{ &s_type,, flt::refl::Type::GetType<Tester<float>>()};}\
		{ static flt::refl::BaseRegister baseRegister{ &s_type,, flt::refl::Type::GetType<Tester<double>>()};}\
		{static flt::refl::Method method{s_type, &Example::Print, "Print", new flt::refl::Callable(&Example::Print)};}\
		{static flt::refl::Method method{s_type, &Example::GetValue, "GetValue", new flt::refl::Callable(&Example::GetValue)};}\
		{static flt::refl::Method method{s_type, &Example::SetValue, "SetValue", new flt::refl::Callable(&Example::SetValue)};}\
	\
		{static flt::refl::Property property{&s_type, {"_valueF", flt::refl::Type::GetType<decltype(_valueF)>(), new flt::refl::PropertyHandler(&Example::_valueF)}};}\
		{static flt::refl::Property property{&s_type, {"_tester", flt::refl::Type::GetType<decltype(_tester)>(), new flt::refl::PropertyHandler(&Example::_tester)}};}\
	\
		return &s_type;\
	}\
	\
private:\
	inline static flt::refl::Type* _type = InitType();

#define FTL_FILE_PATH_HEADERTOOL_EXAMPLE_H_14_FLT_REFL\
	template<typename T>\
	friend struct flt::refl::TypeBuilder;\
public:\
	virtual flt::refl::Type* GetType() const\
	{\
		return _type;\
	}\
\
	static flt::refl::Type* InitType()\
	{\
		static flt::refl::Type s_type{flt::refl::TypeBuilder<InNameSpaceClass>{"InNameSpaceClass"}};\
	\
	\
	\
		return &s_type;\
	}\
	\
private:\
	inline static flt::refl::Type* _type = InitType();

