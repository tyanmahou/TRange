#pragma once
#include<algorithm>
namespace trange
{

	template<class Range, class Pred>
	bool allOf(const Range& range, Pred pred)
	{
		return std::all_of(std::begin(range), std::end(range), pred);
	}

	template<class Range, class Pred>
	bool anyOf(const Range& range, Pred pred)
	{
		return std::any_of(std::begin(range), std::end(range), pred);
	}

	template<class Range, class Pred>
	bool noneOf(const Range& range, Pred pred)
	{
		return std::none_of(std::begin(range), std::end(range), pred);
	}

	template<class Range, class Type = range_value_t<Range>>
	bool contains(const Range& range, const Type& value)
	{
		return std::find(std::begin(range), std::end(range), value) != std::end(range);
	}

	template<class Range1, class Range2>
	bool sequenceEqual(const Range1& range, const Range2& other)
	{
		if (std::size(range) != std::size(other))
			return false;
		auto i1 = std::begin(range);
		auto i2 = std::begin(other);
		for (; i1 != std::end(range) && i2 != std::end(other); ++i1, ++i2)
		{
			if (*i1 != *i2)
			{
				return false;
			}
		}
		return true;
	}
}