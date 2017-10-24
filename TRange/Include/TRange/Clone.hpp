#pragma once
#include<array>

namespace trange
{

	template<class Range>
	Range clone(const Range& range)
	{
		return std::move(range);
	}
	template<class Type, int Size>
	std::array<Type, Size> clone(const Type(&range)[Size])
	{
		std::array<Type, Size>ret;
		for (auto[elm, i] : ret - range::with_index)
		{
			elm = range[i];
		}
		return ret;
	}
}