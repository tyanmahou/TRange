#pragma once
#include<algorithm>
#include<numeric>
#include<vector>
namespace trange
{
	template<class Range>
	decltype(auto) max(Range&& range)
	{
		return *std::max_element(std::begin(range), std::end(range));
	}

	template<class Range>
	decltype(auto) min(Range&& range)
	{
		return *std::min_element(std::begin(range), std::end(range));
	}

	template<class Range>
	decltype(auto) sum(const Range& range)
	{
		return std::accumulate(std::begin(range), std::end(range), 0);
	}

	template<class Range>
	decltype(auto) average(const Range& range)
	{
		return trange::sum(range) / static_cast<double>(std::size(range));
	}

	template<class Range>
	decltype(auto) count(const Range& range)
	{
		return std::size(range);
	}

	template<class Range, class Type, class Pred>
	decltype(auto) aggregate(const Range& range, const Type& init, Pred pred)
	{
		Type result = init;
		for (auto&& elm : range)
		{
			result = pred(result, elm);
		}
		return 	result;
	}


	template<class Range>
	decltype(auto) product(const Range& range)
	{
		return trange::aggregate(range, 1, [](const auto& l, const auto& r) {return l*r; });
	}


	template<class Type>
	decltype(auto) elmentAt(std::vector<Type>& range, std::size_t num)
	{
		std::cout << "Vector";
		return range[num];
	}
	template<class Type,int N>
	decltype(auto) elmentAt(Type(&range)[N], std::size_t num)
	{
		return range[num];
	}
	//template<class Range>
	//decltype(auto) elmentAt(Range&& range, std::size_t num)
	//{
	//	auto it = std::begin(range);
	//	for (std::size_t i = 0; i < num; ++i, ++it)
	//	{
	//	}
	//	return *it;
	//}

}