#undef CURRENT_FILE_PATH
#define CURRENT_FILE_PATH FTL_FILE_PATH_HEADERTOOL_EXAMPLE_H

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

