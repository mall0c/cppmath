#ifndef CPPMATH_COMMON_TYPE_HPP
#define CPPMATH_COMMON_TYPE_HPP

#include <type_traits>

// Drop in replacement for std::common_type because MSVC's implementation is
// buggy and causes compiler errors on certain vector operations
// (e.g. multiplying Vec2f with Vec2d).
// On other platforms than windows std::common_type is used.
// Credits to Zeldhoron (https://github.com/Zeldhoron/)

namespace math
{
#ifdef _WIN32
	namespace detail
	{
        template <typename...>
        using void_t = void;

		template <typename... T> struct common_type;


		template <typename T1, typename T2>
		using common_type_cond_t = decltype(false ? std::declval<T1>() : std::declval<T2>());


		template <typename T1, typename T2, typename = void>
		struct common_type_2_default {};
		template <typename T1, typename T2>
		struct common_type_2_default<T1, T2, void_t<common_type_cond_t<T1, T2>>>
		{
			using type = typename std::decay<common_type_cond_t<T1, T2>>::type;
		};

		template <typename T1, typename T2, typename D1 = typename std::decay<T1>::type, typename D2 = typename std::decay<T2>::type>
		struct common_type_2_impl : common_type<D1, D2> {};
		template <typename D1, typename D2>
		struct common_type_2_impl<D1, D2, D1, D2> : common_type_2_default<D1, D2> {};


		template <typename AlwaysVoid, typename T1, typename T2, typename... R>
		struct common_type_multi_impl {};
		template <typename T1, typename T2, typename... R>
		struct common_type_multi_impl<void_t<typename common_type<T1, T2>::type>, T1, T2, R...> : common_type<typename common_type<T1, T2>::type, R...> {};


		template <>
		struct common_type<> {};
		template <typename T>
		struct common_type<T>
		{
			using type = typename std::decay<T>::type;
		};
		template <typename T1, typename T2>
		struct common_type<T1, T2> : common_type_2_impl<T1, T2> {};
		template <typename T1, typename T2, typename... R>
		struct common_type<T1, T2, R...> : common_type_multi_impl<void, T1, T2, R...> {};
	}

	template <typename... T>
	struct common_type : detail::common_type<T...> {};
#else
	template <typename... T>
    using common_type = std::common_type<T...>;
#endif
}

#endif
