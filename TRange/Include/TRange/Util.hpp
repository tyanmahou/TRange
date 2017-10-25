#pragma once

namespace trange
{
	namespace detail
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
			{
			}
			return i;
		}
	}

	const auto _default = [](auto&& v) {return v; };
	const auto _always_true = [](auto&& v)->bool {return true; };



	constexpr struct _AsConst_OP
	{
		template<class Range>
		const Range operator()(Range&& range)
		{
			return range;
		}
		template<class Range>
		friend const Range operator -(Range&& range, _AsConst_OP)
		{
			return range;
		}
	}asConst;
	
}