#pragma once

namespace trange
{
	namespace detail
	{

		template<class Range>
		using range_value_t = typename std::iterator_traits<decltype(std::begin(std::declval<Range&>()))>::value_type;

		template<class Range>
		using range_iterator_t = decltype(std::begin(std::declval<Range&>()));

		template<class Range>
		using range_const_iterator_t = decltype(std::cbegin(std::declval<Range&>()));

		template<class It>
		std::size_t size(It begin, It end)
		{
			std::size_t i = 0;
			for (auto it = begin; it != end; ++i, ++it)
			{
			}
			return i;
		}
	}

	const auto _default = [](auto&& v) {return v; };
	const auto _always_true = [](auto&& v)->bool {return true; };

	template<class It>
	It advance(It it,std::size_t offset)
	{
		std::advance(it, offset);
		return it;
	}
}