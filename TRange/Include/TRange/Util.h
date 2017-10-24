#pragma once

namespace trange
{

	template<class Range>
	using range_value_t = typename std::iterator_traits<decltype(std::begin(std::declval<Range&>()))>::value_type;

	template<class Range>
	using range_iterator_t = decltype(std::begin(std::declval<Range&>()));

	template<class It>
	std::size_t size(It begin, It end)
	{
		std::size_t i = 0;
		for (auto it = begin; it != end; ++i, ++it)
		{}
		return i;
	}

}