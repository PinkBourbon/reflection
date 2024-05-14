#pragma once
#include <string_view>
#include "hash.h"

#define TYPE_NAME_WIDE_IMPL(x) L ## x
#define TYPE_NAME_WIDE(x) TYPE_NAME_WIDE_IMPL(x)
#define TYPE_NAME_WFUNCSIG TYPE_NAME_WIDE(__FUNCSIG__)



//template<>
//struct TypeName<std::string>
//{
//	static constexpr std::string_view value{ "std::string" };
//	static constexpr unsigned int hash{};
//};
//
//template<>
//struct TypeName<std::wstring>
//{
//	static constexpr std::string_view value{ "std::wstring" };
//	static constexpr unsigned int hash{};
//};

namespace flt
{
	template<class T>
	struct TypeName
	{
	private:
		static constexpr std::string_view get_name() noexcept
		{
			// msvc 19.21 이상에서만 동작
#if (_MSC_VER >= 1921)
			constexpr std::string_view full_name{ __FUNCSIG__ };
			constexpr std::string_view prefix{ "TypeName<" };
			constexpr std::string_view suffix{ ">::get_name(" };

			constexpr auto left_marker_index = full_name.find(prefix);
			static_assert(left_marker_index != std::string_view::npos);
			constexpr auto start_index = left_marker_index + prefix.size();
			constexpr auto end_index = full_name.find(suffix, left_marker_index);
			static_assert(end_index != std::string_view::npos);
			constexpr auto length = end_index - start_index;

			return full_name.substr(start_index, length);
#endif
		}
	public:
		static constexpr std::string_view value{ get_name() };
		static constexpr unsigned long long hash{ xx::xxh64::hash(value.data(), value.length(), 7) };
	};

	template<class T>
	inline constexpr auto TypeName_v = TypeName<T>::value;

	template<class T>
	struct WTypeName
	{
	private:
		static constexpr std::wstring_view get_name() noexcept
		{
			// msvc 19.21 이상에서만 동작
#if (_MSC_VER >= 1921)
			constexpr std::wstring_view full_name{ TYPE_NAME_WFUNCSIG };
			constexpr std::wstring_view prefix{ L"WTypeName<" };
			constexpr std::wstring_view suffix{ L">::get_name(" };

			constexpr auto left_marker_index = full_name.find(prefix);
			static_assert(left_marker_index != std::wstring_view::npos);
			constexpr auto start_index = left_marker_index + prefix.size();
			constexpr auto end_index = full_name.find(suffix, left_marker_index);
			static_assert(end_index != std::wstring_view::npos);
			constexpr auto length = end_index - start_index;

			return full_name.substr(start_index, length);
#endif
		}
	public:
		static constexpr std::wstring_view value{ get_name() };
		static constexpr unsigned int hash{};
	};

	template<class T>
	inline constexpr auto WTypeName_v = WTypeName<T>::value;

}
